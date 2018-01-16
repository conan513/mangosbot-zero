#include "immersivepch.h"
#include "immersive.h"
#include "../../game/Server/SharedDefines.h"

using namespace immersive;

map<Stats, string> Immersive::statValues;

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
    switch (gossipListId)
    {
    case 0: // Help
        PrintHelp(player, true);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        IncreaseStat(player, gossipListId - 1);
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

    ChatHandler(player->GetSession()).PSendSysMessage("Attribute points available: %d (%d used)", (totalStats - usedStats), usedStats);
    ChatHandler(player->GetSession()).PSendSysMessage("Training cost: %s", formatMoney(cost));

    if (detailed)
    {
        for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
            ChatHandler(player->GetSession()).PSendSysMessage("%s: %d", statValues[(Stats)i].c_str(), GetValue(owner, statValues[(Stats)i]));
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
        ChatHandler(player->GetSession()).PSendSysMessage("You have no attribute points left");
        return;
    }

    if (player->GetMoney() < cost)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("You have not enough gold");
        return;
    }

    uint32 value = GetValue(owner, statValues[(Stats)type]);
    SetValue(owner, statValues[(Stats)type], value + 1);
    ChatHandler(player->GetSession()).PSendSysMessage("You have gained +1 %s", statValues[(Stats)type].c_str());
    player->InitStatsForLevel(true);
    player->UpdateAllStats();
    player->ModifyMoney(-cost);
    player->SaveGoldToDB();

    PrintHelp(player);
}

void Immersive::ResetStats(Player *player)
{
    uint32 owner = player->GetObjectGuid().GetRawValue();

    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        SetValue(owner, statValues[(Stats)i], 0);

    ChatHandler(player->GetSession()).PSendSysMessage("Your attribute points have been reset");
    player->InitStatsForLevel(true);
    player->UpdateAllStats();

    PrintHelp(player);
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
