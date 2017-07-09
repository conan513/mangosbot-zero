#include "botpch.h"
#include "../../playerbot.h"
#include "ListSpellsAction.h"
#include "../ItemVisitors.h"

using namespace ai;

int SortSpells(pair<uint32, string>& s1, pair<uint32, string>& s2)
{
    int p1 = IsPositiveSpell(s1.first) ? 1 : 0;
    int p2 = IsPositiveSpell(s2.first) ? 1 : 0;
    if (p1 == p2)
    {
        const SpellEntry* const si1 = sSpellStore.LookupEntry(s1.first);
        const SpellEntry* const si2 = sSpellStore.LookupEntry(s2.first);
        p1 = si1->School;
        p2 = si2->School;

        if (p1 == p2)
        {
            uint32 skill1 = 0, skill2 = 0;
            uint32 skillValue1 = 0, skillValue2 = 0;
            for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
            {
                SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
                if (skillLine && skillLine->spellId == s1.first)
                {
                    skill1 = skillLine->skillId;
                    skillValue1 = skillLine->min_value;
                }
                if (skillLine && skillLine->spellId == s2.first)
                {
                    skill2 = skillLine->skillId;
                    skillValue2 = skillLine->min_value;
                }
                if (skill1 && skill2) break;
            }

            p1 = skill1;
            p2 = skill2;
            if (p1 == p2)
            {
                p1 = skillValue1;
                p2 = skillValue2;

                if (p1 == p2)
                {
                    return strcmp(si1->SpellName[0], si1->SpellName[1]);
                }
            }

        }
    }

    return p1 = p2;
}

bool ListSpellsAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    int loc = master->GetSession()->GetSessionDbcLocale();

    std::ostringstream posOut;
    std::ostringstream negOut;

    string filter = event.getParam();
    uint32 skill = chat->parseSkill(filter);
    if (skill != SKILL_NONE) filter = "";

    list<uint32> skillSpells;
    for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
    {
        SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
        if (!skillLine || skillLine->skillId != skill)
            continue;

        skillSpells.push_back(skillLine->spellId);
    }

    const std::string ignoreList = ",Opening,Closing,Stuck,Remove Insignia,Opening - No Text,Grovel,Duel,Honorless Target,";
    std::string alreadySeenList = ",";

    list<pair<uint32, string> > spells;
    for (PlayerSpellMap::iterator itr = bot->GetSpellMap().begin(); itr != bot->GetSpellMap().end(); ++itr) {
        const uint32 spellId = itr->first;

        if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled || IsPassiveSpell(spellId))
            continue;

        const SpellEntry* const pSpellInfo = sSpellStore.LookupEntry(spellId);
        if (!pSpellInfo)
            continue;

        if (skill != SKILL_NONE && find(skillSpells.begin(), skillSpells.end(), spellId) == skillSpells.end())
            continue;

        if (skill == SKILL_NONE && find(skillSpells.begin(), skillSpells.end(), spellId) != skillSpells.end())
            continue;

        string comp = pSpellInfo->SpellName[0];
        if (!(ignoreList.find(comp) == std::string::npos && alreadySeenList.find(comp) == std::string::npos))
            continue;

        if (!filter.empty() && !strstri(pSpellInfo->SpellName[loc], filter.c_str()))
            continue;

        ostringstream out;
        out << chat->formatSpell(pSpellInfo);

        bool first = true;
        for (uint32 x = 0; x < MAX_SPELL_REAGENTS; ++x)
        {
            if (pSpellInfo->Reagent[x] <= 0)
                { continue; }

            uint32 itemid = pSpellInfo->Reagent[x];
            uint32 itemcount = pSpellInfo->ReagentCount[x];
            if (itemid)
            {
                ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemid);
                if (proto)
                {
                    if (first) { out << ": "; first = false; } else out << ", ";
                    out << chat->formatItem(proto, itemcount);
                }
            }
        }

        spells.push_back(pair<uint32, string>(spellId, out.str()));
        alreadySeenList += pSpellInfo->SpellName[loc];
        alreadySeenList += ",";
    }

    ai->TellMaster("=== Spells ===");
    spells.sort(SortSpells);

    for (list<pair<uint32, string> >::iterator i = spells.begin(); i != spells.end(); ++i)
    {
        ai->TellMaster(i->second);
    }

    return true;
}

