#include "botpch.h"
#include "../../playerbot.h"
#include "ListSpellsAction.h"
#include "../ItemVisitors.h"

using namespace ai;

map<uint32, SkillLineAbilityEntry const*> ListSpellsAction::skillSpells;

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

    if (skillSpells.empty())
    {
        for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
        {
            SkillLineAbilityEntry const* skillLine = sSkillLineAbilityStore.LookupEntry(j);
            if (skillLine)
                skillSpells[skillLine->spellId] = skillLine;
        }
    }

    std::ostringstream posOut;
    std::ostringstream negOut;

    string filter = event.getParam();
    uint32 skill = 0;

    vector<string> ss = split(filter, ' ');
    if (!ss.empty())
    {
        skill = chat->parseSkill(ss[0]);
        if (skill != SKILL_NONE)
        {
            filter = ss.size() > 1 ? filter = ss[1] : "";
        }

        if (ss[0] == "first" && ss[1] == "aid")
        {
            skill = SKILL_FIRST_AID;
            filter = ss.size() > 2 ? filter = ss[2] : "";
        }
    }


    const std::string ignoreList = ",Opening,Closing,Stuck,Remove Insignia,Opening - No Text,Grovel,Duel,Honorless Target,";
    std::string alreadySeenList = ",";

    int minLevel = 0, maxLevel = 0;
    if (filter.find("-") != string::npos)
    {
        vector<string> ff = split(filter, '-');
        minLevel = atoi(ff[0].c_str());
        maxLevel = atoi(ff[1].c_str());
        filter = "";
    }

    bool craftableOnly = false;
    if (filter.find("+") != string::npos)
    {
        craftableOnly = true;
        filter.erase(remove(filter.begin(), filter.end(), '+'), filter.end());
    }

    uint32 slot = chat->parseSlot(filter);
    if (slot != EQUIPMENT_SLOT_END)
        filter = "";

    list<pair<uint32, string> > spells;
    for (PlayerSpellMap::iterator itr = bot->GetSpellMap().begin(); itr != bot->GetSpellMap().end(); ++itr) {
        const uint32 spellId = itr->first;

        if (itr->second.state == PLAYERSPELL_REMOVED || itr->second.disabled || IsPassiveSpell(spellId))
            continue;

        const SpellEntry* const pSpellInfo = sSpellStore.LookupEntry(spellId);
        if (!pSpellInfo)
            continue;

        SkillLineAbilityEntry const* skillLine = skillSpells[spellId];
        if (skill != SKILL_NONE && (!skillLine || skillLine->skillId != skill))
            continue;

        string comp = pSpellInfo->SpellName[0];
        if (!(ignoreList.find(comp) == std::string::npos && alreadySeenList.find(comp) == std::string::npos))
            continue;

        if (!filter.empty() && !strstri(pSpellInfo->SpellName[0], filter.c_str()))
            continue;

        bool first = true;
        int craftCount = 0;
        ostringstream materials;
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
                    if (first) { materials << ": "; first = false; } else materials << ", ";
                    materials << chat->formatItem(proto, itemcount);

                    FindItemByIdVisitor visitor(itemid);
                    uint32 reagentCount = InventoryAction::GetItemCount(&visitor);
                    uint32 craftable = reagentCount / itemcount;
                    if (!craftCount || craftCount > craftable)
                        craftCount = craftable;
                    if (reagentCount)
                        materials << " (x" << reagentCount << ")";
                }
            }
        }

        ostringstream out;
        bool filtered = false;
        if (skillLine)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (pSpellInfo->Effect[i] == SPELL_EFFECT_CREATE_ITEM)
                {
                    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(pSpellInfo->EffectItemType[i]);
                    if (proto)
                    {
                        if (craftCount)
                            out << "|cffffff00(x" << craftCount << ")|r ";
                        out << chat->formatItem(proto);

                        if ((minLevel || maxLevel) && (!proto->RequiredLevel || proto->RequiredLevel < minLevel || proto->RequiredLevel > maxLevel))
                        {
                            filtered = true;
                            break;
                        }

                        if (slot != EQUIPMENT_SLOT_END && bot->FindEquipSlot(proto, slot, true) != slot)
                        {
                            filtered = true;
                            break;
                        }
                    }
                }
            }
        }

        if (out.str().empty())
            out << chat->formatSpell(pSpellInfo);

        if (filtered)
            continue;

        if (craftableOnly && !craftCount)
            continue;

        out << materials.str();

        if (out.str().empty())
            continue;

        spells.push_back(pair<uint32, string>(spellId, out.str()));
        alreadySeenList += pSpellInfo->SpellName[0];
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

