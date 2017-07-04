#include "botpch.h"
#include "../../playerbot.h"
#include "GoAction.h"
#include "../../PlayerbotAIConfig.h"
#include "../values/Formations.h"
#include "../values/PositionValue.h"

using namespace ai;

vector<string> split(const string &s, char delim);

bool GoAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    string param = event.getParam();
    if (param == "?")
    {
        float x = bot->GetPositionX();
        float y = bot->GetPositionY();
        Map2ZoneCoordinates(x, y, bot->GetZoneId());
        ostringstream out;
        out << "I am at " << x << "," << y;
        ai->TellMaster(out.str());
        return true;
    }

    list<ObjectGuid> gos = ChatHelper::parseGameobjects(param);
    if (!gos.empty())
    {
        for (list<ObjectGuid>::iterator i = gos.begin(); i != gos.end(); ++i)
        {
            GameObject* go = ai->GetGameObject(*i);
            if (go && go->isSpawned())
            {
                if (bot->GetDistance2d(go) > sPlayerbotAIConfig.reactDistance)
                {
                    ai->TellMaster("It is too far away");
                    return false;
                }

                ostringstream out; out << "Moving to " << ChatHelper::formatGameobject(go);
                ai->TellMasterNoFacing(out.str());
                return MoveNear(bot->GetMapId(), go->GetPositionX(), go->GetPositionY(), go->GetPositionZ() + 0.5f, sPlayerbotAIConfig.followDistance);
            }
        }
        return false;
    }

    if (param.find(",") != string::npos)
    {
        vector<string> coords = split(param, ',');
        float x = atof(coords[0].c_str());
        float y = atof(coords[1].c_str());
        Zone2MapCoordinates(x, y, bot->GetZoneId());

        Map* map = bot->GetMap();
        float z = bot->GetPositionZ();
        bot->UpdateGroundPositionZ(x, y, z);

        if (bot->GetDistance2d(x, y) > sPlayerbotAIConfig.reactDistance)
        {
            ai->TellMaster("It is too far away");
            return false;
        }

        const TerrainInfo* terrain = map->GetTerrain();
        if (terrain->IsUnderWater(x, y, z) || terrain->IsInWater(x, y, z))
        {
            ai->TellMaster("It is under water");
            return false;
        }

        float ground = map->GetHeight(x, y, z + 0.5f);
        if (ground <= INVALID_HEIGHT)
        {
            ai->TellMaster("I can't go there");
            return false;
        }

        ostringstream out; out << "Moving to " << x << "," << y;
        ai->TellMasterNoFacing(out.str());
        return MoveNear(bot->GetMapId(), x, y, z + 0.5f, sPlayerbotAIConfig.followDistance);
    }

    ai::Position pos = context->GetValue<ai::PositionMap&>("position")->Get()[param];
    if (pos.isSet())
    {
        if (bot->GetDistance2d(pos.x, pos.y) > sPlayerbotAIConfig.reactDistance)
        {
            ai->TellMaster("It is too far away");
            return false;
        }

        ostringstream out; out << "Moving to position " << param;
        ai->TellMasterNoFacing(out.str());
        return MoveNear(bot->GetMapId(), pos.x, pos.y, pos.z + 0.5f, sPlayerbotAIConfig.followDistance);
    }

    ai->TellMaster("Whisper 'go x,y', 'go [game object]' or 'go position' and I will go there");
    return false;
}
