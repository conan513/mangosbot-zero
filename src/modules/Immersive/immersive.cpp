#include "immersivepch.h"
#include "immersive.h"
#include "../../game/Server/SharedDefines.h"

using namespace immersive;

map<Stats, string> Immersive::statValues;
map<Stats, string> Immersive::statNames;

string formatMoney(uint32 copper)
{
    ostringstream out;
    if (!copper)
    {
        out << "0";
        return out.str();
    }

    uint32 gold = uint32(copper / 10000);
    copper -= (gold * 10000);
    uint32 silver = uint32(copper / 100);
    copper -= (silver * 100);

    bool space = false;
    if (gold > 0)
    {
        out << gold <<  "g";
        space = true;
    }

    if (silver > 0 && gold < 50)
    {
        if (space) out << " ";
        out << silver <<  "s";
        space = true;
    }

    if (copper > 0 && gold < 10)
    {
        if (space) out << " ";
        out << copper <<  "c";
    }

    return out.str();
}
Immersive::Immersive()
{
    statValues[STAT_STRENGTH] = "Strength";
    statValues[STAT_AGILITY] = "Agility";
    statValues[STAT_STAMINA] = "Stamina";
    statValues[STAT_INTELLECT] = "Intellect";
    statValues[STAT_SPIRIT] = "Spirit";

    statNames[STAT_STRENGTH] = "STR";
    statNames[STAT_AGILITY] = "AGI";
    statNames[STAT_STAMINA] = "STA";
    statNames[STAT_INTELLECT] = "INT";
    statNames[STAT_SPIRIT] = "SPI";
}

void Immersive::GetPlayerLevelInfo(Player *player, PlayerLevelInfo* info)
{
    PlayerInfo const* playerInfo = sObjectMgr.GetPlayerInfo(player->getRace(), player->getClass());
    *info = playerInfo->levelInfo[0];

    uint32 owner = player->GetObjectGuid().GetRawValue();
    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        info->stats[i] += GetValue(owner, Immersive::statValues[(Stats)i]);
}

void Immersive::OnGossipSelect(Player *player, uint32 gossipListId, GossipMenuItemData *menuData)
{
    switch (menuData->m_gAction_poi)
    {
    case 0: // Help
        PrintHelp(player, true);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        IncreaseStat(player, menuData->m_gAction_poi - 1);
        break;
    case 6:
        ResetStats(player);
        break;
    }
}

void Immersive::PrintHelp(Player *player, bool detailed)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);

    ChatHandler &chat = ChatHandler(player->GetSession());
    chat.PSendSysMessage("|cffa0a0ff== Attribute Points ==");
    chat.PSendSysMessage("|cffa0a0ffAvailable: |cff00ff00%d|cffa0a0ff (|cffffff00%s|cffa0a0ff per use)", (totalStats - usedStats), formatMoney(cost));

    if (detailed)
    {
        ostringstream out;
        out << "|cffa0a0ffUsed: ";
        bool first = true;
        bool used = false;
        for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        {
            uint32 value = GetValue(owner, statValues[(Stats)i]);
            if (!value) continue;
            if (!first) out << ", "; else first = false;
            out << "|cff00ff00+" << value << "|cffa0a0ff " << statNames[(Stats)i];
            used = true;
        }
        if (used)
            chat.PSendSysMessage(out.str().c_str());
    }
}

void Immersive::IncreaseStat(Player *player, uint32 type)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);

    if (usedStats >= totalStats)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("|cffffa0a0You have no attribute points left");
        return;
    }

    if (player->GetMoney() < cost)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("|cffffa0a0You have not enough gold");
        return;
    }

    uint32 value = GetValue(owner, statValues[(Stats)type]);
    SetValue(owner, statValues[(Stats)type], value + 1);

    usedStats = GetUsedStats(player);
    totalStats = GetTotalStats(player);
    cost = GetStatCost(player);
    ChatHandler(player->GetSession()).PSendSysMessage("|cffa0a0ffYou have gained |cff00ff00+1|cffa0a0ff %s, |cff00ff00%d|cffa0a0ff points left (|cffffff00%s|cffa0a0ff per use)",
            statNames[(Stats)type].c_str(), (totalStats - usedStats), formatMoney(cost));

    player->InitStatsForLevel(true);
    player->UpdateAllStats();
    player->ModifyMoney(-cost);
    player->SaveGoldToDB();
}

void Immersive::ResetStats(Player *player)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        SetValue(owner, statValues[(Stats)i], 0);

    uint32 usedStats = GetUsedStats(player);
    uint32 totalStats = GetTotalStats(player);
    uint32 cost = GetStatCost(player);
    ChatHandler(player->GetSession()).PSendSysMessage("|cffa0a0ffYour attributes have been reset, |cff00ff00%d|cffa0a0ff points available (|cffffff00%s|cffa0a0ff per use)",
            (totalStats - usedStats), formatMoney(cost));
    player->InitStatsForLevel(true);
    player->UpdateAllStats();
}

uint32 Immersive::GetTotalStats(Player *player)
{
    return player->getLevel() * 3;
}

uint32 Immersive::GetUsedStats(Player *player)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    uint32 usedStats = 0;
    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        usedStats += GetValue(owner, statValues[(Stats)i]);

    return usedStats;
}

uint32 Immersive::GetStatCost(Player *player)
{
    uint32 usedLevels = GetUsedStats(player) / 3;
    return 10 * (usedLevels * usedLevels + 1);
}

uint32 Immersive::GetValue(uint32 owner, string type)
{
    uint32 value = 0;

    QueryResult* results = CharacterDatabase.PQuery(
            "select `value` from immersive_values where owner = '%u' and `type` = '%s'",
            owner, type.c_str());

    if (results)
    {
        Field* fields = results->Fetch();
        value = fields[0].GetUInt32();
    }

    delete results;
    return value;
}

uint32 Immersive::SetValue(uint32 owner, string type, uint32 value)
{
    CharacterDatabase.DirectPExecute("delete from immersive_values where owner = '%u' and `type` = '%s'",
            owner, type.c_str());
    if (value)
    {
        CharacterDatabase.DirectPExecute(
                "insert into immersive_values (owner, `type`, `value`) values ('%u', '%s', '%u')",
                owner, type.c_str(), value);
    }

    return value;
}


INSTANTIATE_SINGLETON_1( immersive::Immersive );
