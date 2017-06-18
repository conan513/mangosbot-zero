#include "botpch.h"
#include "../../playerbot.h"
#include "CastCustomSpellAction.h"

#include "../../PlayerbotAIConfig.h"
using namespace ai;

bool CastCustomSpellAction::Execute(Event event)
{
    Unit* target = NULL;

    Player* master = GetMaster();
    if (master && master->GetSelectionGuid())
        target = ai->GetUnit(master->GetSelectionGuid());

    if (!target)
        target = bot;

    string text = event.getParam();

    uint32 spell = AI_VALUE2(uint32, "spell id", text);

    ostringstream msg;
    if (target != bot && !bot->IsInFront(target, sPlayerbotAIConfig.sightDistance, M_PI / 2))
    {
        bot->SetFacingTo(bot->GetAngle(target));
        ai->SetNextCheckDelay(sPlayerbotAIConfig.globalCoolDown);
        msg << "cast " << text;
        ai->HandleCommand(CHAT_MSG_WHISPER, msg.str(), *master);
        return true;
    }

    if (!ai->CanCastSpell(spell, target))
    {
        msg << "Cannot cast " << text << " on " << target->GetName();
        ai->TellMaster(msg.str());
        return false;
    }

    bool result = spell ? ai->CastSpell(spell, target) : ai->CastSpell(text, target);
    if (result)
    {
        msg << "Casting " << text << " on " << target->GetName();
        ai->TellMasterNoFacing(msg.str());
    }
    else
    {
        msg << "Cast " << text << " on " << target->GetName() << " is failed";
        ai->TellMaster(msg.str());
    }

    return result;
}
