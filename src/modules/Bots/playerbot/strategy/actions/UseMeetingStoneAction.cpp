#include "botpch.h"
#include "../../playerbot.h"
#include "UseMeetingStoneAction.h"
#include "../../PlayerbotAIConfig.h"

#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

using namespace MaNGOS;

bool UseMeetingStoneAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    WorldPacket p(event.getPacket());
    p.rpos(0);
    ObjectGuid guid;
    p >> guid;

	if (master->GetSelectionGuid() && master->GetSelectionGuid() != bot->GetObjectGuid())
		return false;

	if (!master->GetSelectionGuid() && master->GetGroup() != bot->GetGroup())
		return false;

    if (master->IsBeingTeleported())
        return false;

    if (bot->IsInCombat())
    {
        ai->TellMasterNoFacing("I am in combat");
        return false;
    }

    Map* map = master->GetMap();
    if (!map)
        return NULL;

    GameObject *gameObject = map->GetGameObject(guid);
    if (!gameObject)
        return false;

	const GameObjectInfo* goInfo = gameObject->GetGOInfo();
	if (!goInfo || goInfo->type != GAMEOBJECT_TYPE_SUMMONING_RITUAL)
        return false;

    return Teleport();
}

class AnyGameObjectInObjectRangeCheck
{
public:
    AnyGameObjectInObjectRangeCheck(WorldObject const* obj, float range) : i_obj(obj), i_range(range) {}
    WorldObject const& GetFocusObject() const { return *i_obj; }
    bool operator()(GameObject* u)
    {
        if (u && i_obj->IsWithinDistInMap(u, i_range) && u->isSpawned() && u->GetGOInfo())
            return true;

        return false;
    }

private:
    WorldObject const* i_obj;
    float i_range;
};


bool SummonAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    if (master->GetSession()->GetSecurity() >= SEC_GAMEMASTER)
        return Teleport();

    list<GameObject*> targets;

    AnyGameObjectInObjectRangeCheck u_check(master, sPlayerbotAIConfig.sightDistance);
    GameObjectListSearcher<AnyGameObjectInObjectRangeCheck> searcher(targets, u_check);
    Cell::VisitAllObjects((const WorldObject*)master, searcher, sPlayerbotAIConfig.sightDistance);

    list<ObjectGuid> result;
    for(list<GameObject*>::iterator tIter = targets.begin(); tIter != targets.end(); ++tIter)
    {
        GameObject* go = *tIter;
        if (go && go->isSpawned() && go->GetGoType() == GAMEOBJECT_TYPE_MEETINGSTONE)
            return Teleport();
    }

    ai->TellMasterNoFacing("There are no meeting stone nearby");
    return false;
}

bool SummonAction::Teleport()
{
    Player* master = GetMaster();
    if (!master->IsBeingTeleported())
    {
        float followAngle = GetFollowAngle();
        for (float angle = followAngle - M_PI; angle <= followAngle + M_PI; angle += M_PI / 4)
        {
            uint32 mapId = master->GetMapId();
            float x = master->GetPositionX() + cos(angle) * sPlayerbotAIConfig.followDistance;
            float y = master->GetPositionY()+ sin(angle) * sPlayerbotAIConfig.followDistance;
            float z = master->GetPositionZ();
            if (master->IsWithinLOS(x, y, z))
            {
                bot->GetMotionMaster()->Clear();
                bot->TeleportTo(mapId, x, y, z, 0);
                return true;
            }
        }
    }

    ai->TellMasterNoFacing("There is not enough place to summon me");
    return false;
}
