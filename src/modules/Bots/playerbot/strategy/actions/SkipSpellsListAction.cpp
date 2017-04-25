#include "botpch.h"
#include "../../playerbot.h"
#include "SkipSpellsListAction.h"
#include "../values/SkipSpellsListValue.h"
#include "LootAction.h"

using namespace ai;


bool SkipSpellsListAction::Execute(Event event)
{
    string cmd = event.getParam();

    set<uint32>& skipSpells = AI_VALUE(set<uint32>&, "skip spells list");

    if (cmd == "?")
    {
        ostringstream out;
        out << "I won't cast: ";

        for (set<uint32>::iterator i = skipSpells.begin(); i != skipSpells.end(); i++)
        {
            SpellEntry const* spell = sSpellStore.LookupEntry(*i);
            if (!spell)
                continue;

            out << chat->formatSpell(spell);
        }
        ai->TellMaster(out);
    }
    else
    {
        bool remove = cmd.size() > 1 && cmd.substr(0, 1) == "-";
        if (remove)
            cmd = cmd.substr(1);

        uint32 spellId = chat->parseSpell(cmd);
        if (!spellId)
        {
            ai->TellMaster("Unknown spell");
            return false;
        }

        SpellEntry const* spell = sSpellStore.LookupEntry(spellId);
        if (!spell)
            return false;

        if (remove)
        {
            set<uint32>::iterator j = skipSpells.find(spellId);
            if (j != skipSpells.end())
            {
                skipSpells.erase(j);
                ostringstream out;
                out << chat->formatSpell(spell) << "removed from ignored spells";
                ai->TellMaster(out);
                return true;
            }
        }
        else
        {
            set<uint32>::iterator j = skipSpells.find(spellId);
            if (j == skipSpells.end())
            {
                skipSpells.insert(spellId);
                ostringstream out;
                out << chat->formatSpell(spell) << "added to ignored spells";
                ai->TellMaster(out);
                return true;
            }
        }
    }

    return false;
}

