#include "../botpch.h"
#include "playerbot.h"
#include "PlayerbotAIConfig.h"
#include "GuildTaskMgr.h"

#include "../../modules/Bots/ahbot/AhBot.h"
#include "GuildMgr.h"
#include "DatabaseEnv.h"
#include "Mail.h"
#include "PlayerbotAI.h"

#include "../../modules/Bots/ahbot/AhBotConfig.h"
#include "RandomItemMgr.h"

INSTANTIATE_SINGLETON_1(GuildTaskMgr);

char * strstri (const char* str1, const char* str2);

enum GuildTaskType
{
    GUILD_TASK_TYPE_NONE = 0,
    GUILD_TASK_TYPE_ITEM = 1,
    GUILD_TASK_TYPE_KILL = 2
};

GuildTaskMgr::GuildTaskMgr()
{
}

GuildTaskMgr::~GuildTaskMgr()
{
}

void GuildTaskMgr::Update(Player* player, Player* guildMaster)
{
    if (!sPlayerbotAIConfig.guildTaskEnabled)
        return;

    uint32 guildId = guildMaster->GetGuildId();
    if (!guildId || !guildMaster->GetPlayerbotAI() || !guildMaster->GetGuildId())
        return;

    if (!player->IsFriendlyTo(guildMaster))
        return;

	Guild *guild = sGuildMgr.GetGuildById(guildMaster->GetGuildId());
    DenyReason reason = PLAYERBOT_DENY_NONE;
    PlayerbotSecurityLevel secLevel = guildMaster->GetPlayerbotAI()->GetSecurity()->LevelFor(player, &reason);
    if (secLevel == PLAYERBOT_SECURITY_DENY_ALL || (secLevel == PLAYERBOT_SECURITY_TALK && reason != PLAYERBOT_DENY_FAR))
    {
        sLog.outDebug("%s / %s: skipping guild task update - not enough security level, reason = %u",
			guild->GetName(), player->GetName(), reason);
        return;
    }

    uint32 owner = (uint32)player->GetGUID();

    uint32 activeTask = GetTaskValue(owner, guildId, "activeTask");
    if (!activeTask)
    {
        SetTaskValue(owner, guildId, "killTask", 0, 0);
        SetTaskValue(owner, guildId, "itemTask", 0, 0);
        SetTaskValue(owner, guildId, "itemCount", 0, 0);
        SetTaskValue(owner, guildId, "killTask", 0, 0);
        SetTaskValue(owner, guildId, "killCount", 0, 0);
        SetTaskValue(owner, guildId, "payment", 0, 0);
        SetTaskValue(owner, guildId, "thanks", 1, 2 * sPlayerbotAIConfig.maxGuildTaskChangeTime);
        SetTaskValue(owner, guildId, "reward", 1, 2 * sPlayerbotAIConfig.maxGuildTaskChangeTime);

        uint32 task = CreateTask(owner, guildId);

        if (task == GUILD_TASK_TYPE_NONE)
        {
            sLog.outError( "%s / %s: error creating guild task",
				guild->GetName(), player->GetName());
        }

        uint32 time = urand(sPlayerbotAIConfig.minGuildTaskChangeTime, sPlayerbotAIConfig.maxGuildTaskChangeTime);
        SetTaskValue(owner, guildId, "activeTask", task, time);
        SetTaskValue(owner, guildId, "advertisement", 1,
                urand(sPlayerbotAIConfig.minGuildTaskAdvertisementTime, sPlayerbotAIConfig.maxGuildTaskAdvertisementTime));

        sLog.outDebug("%s / %s: guild task %u is set for %u secs",
				guild->GetName(), player->GetName(),
                task, time);
        return;
    }

    uint32 advertisement = GetTaskValue(owner, guildId, "advertisement");
    if (!advertisement)
    {
        sLog.outDebug("%s / %s: sending advertisement",
				guild->GetName(), player->GetName());
        if (SendAdvertisement(owner, guildId))
        {
            SetTaskValue(owner, guildId, "advertisement", 1,
                    urand(sPlayerbotAIConfig.minGuildTaskAdvertisementTime, sPlayerbotAIConfig.maxGuildTaskAdvertisementTime));
        }
        else
        {
            sLog.outError( "%s / %s: error sending advertisement",
					guild->GetName(), player->GetName());
        }
    }

    uint32 thanks = GetTaskValue(owner, guildId, "thanks");
    if (!thanks)
    {
        sLog.outDebug("%s / %s: sending thanks",
				guild->GetName(), player->GetName());
        if (SendThanks(owner, guildId))
        {
            SetTaskValue(owner, guildId, "thanks", 1, 2 * sPlayerbotAIConfig.maxGuildTaskChangeTime);
            SetTaskValue(owner, guildId, "payment", 0, 0);
        }
        else
        {
            sLog.outError( "%s / %s: error sending thanks",
					guild->GetName(), player->GetName());
        }
    }

    uint32 reward = GetTaskValue(owner, guildId, "reward");
    if (!reward)
    {
        sLog.outDebug("%s / %s: sending reward",
				guild->GetName(), player->GetName());
        if (Reward(owner, guildId))
        {
            SetTaskValue(owner, guildId, "reward", 1, 2 * sPlayerbotAIConfig.maxGuildTaskChangeTime);
            SetTaskValue(owner, guildId, "payment", 0, 0);
        }
        else
        {
            sLog.outError( "%s / %s: error sending reward",
					guild->GetName(), player->GetName());
        }
    }
}

uint32 GuildTaskMgr::CreateTask(uint32 owner, uint32 guildId)
{
    switch (urand(0, 1))
    {
    case 0:
        CreateItemTask(owner, guildId);
        return GUILD_TASK_TYPE_ITEM;
    default:
        CreateKillTask(owner, guildId);
        return GUILD_TASK_TYPE_KILL;
    }
}

bool GuildTaskMgr::CreateItemTask(uint32 owner, uint32 guildId)
{
    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    if (!player)
        return false;

    uint32 itemId = sRandomItemMgr.GetRandomItem(RANDOM_ITEM_GUILD_TASK);
    if (!itemId)
    {
        sLog.outError( "%s / %s: no items avaible for item task",
                sGuildMgr.GetGuildById(guildId)->GetName().c_str(), player->GetName());
        return false;
    }

    uint32 count = GetMaxItemTaskCount(itemId);

    sLog.outDebug("%s / %s: item task %u (x%d)",
            sGuildMgr.GetGuildById(guildId)->GetName().c_str(), player->GetName(),
            itemId, count);

    SetTaskValue(owner, guildId, "itemCount", count, sPlayerbotAIConfig.maxGuildTaskChangeTime);
    SetTaskValue(owner, guildId, "itemTask", itemId, sPlayerbotAIConfig.maxGuildTaskChangeTime);
    return true;
}

bool GuildTaskMgr::CreateKillTask(uint32 owner, uint32 guildId)
{
    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    if (!player)
        return false;

    uint32 rank = !urand(0, 2) ? CREATURE_ELITE_RAREELITE : CREATURE_ELITE_RARE;
    vector<uint32> ids;
	for (uint32 id = 0; id < sCreatureStorage.GetMaxEntry(); ++id)
	{
		CreatureInfo const* co = sCreatureStorage.LookupEntry<CreatureInfo>(id);
		if (!co)
			continue;

        if (co->Rank != rank)
            continue;

        if (co->MaxLevel > player->getLevel() + 4 || co->MinLevel < player->getLevel() - 3)
            continue;

        if (strstr(co->Name, "UNUSED"))
            continue;

        ids.push_back(id);
    }

    if (ids.empty())
    {
        sLog.outError( "%s / %s: no rare creatures available for kill task",
                sGuildMgr.GetGuildById(guildId)->GetName().c_str(), player->GetName());
        return false;
    }

    uint32 index = urand(0, ids.size() - 1);
    uint32 creatureId = ids[index];

    sLog.outDebug("%s / %s: kill task %u",
            sGuildMgr.GetGuildById(guildId)->GetName().c_str(), player->GetName(),
            creatureId);

    SetTaskValue(owner, guildId, "killTask", creatureId, sPlayerbotAIConfig.maxGuildTaskChangeTime);
    return true;
}

bool GuildTaskMgr::SendAdvertisement(uint32 owner, uint32 guildId)
{
    Guild *guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
        return false;

    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    if (!player)
        return false;

    Player* leader = sObjectMgr.GetPlayer(guild->GetLeaderGuid());
    if (!leader)
        return false;

    uint32 validIn;
    uint32 itemTask = GetTaskValue(owner, guildId, "itemTask", &validIn);
    if (itemTask)
        return SendItemAdvertisement(itemTask, owner, guildId, validIn);

    uint32 killTask = GetTaskValue(owner, guildId, "killTask", &validIn);
    if (killTask)
        return SendKillAdvertisement(killTask, owner, guildId, validIn);

    return false;
}

string formatTime(uint32 secs)
{
    ostringstream out;
    if (secs < 3600)
    {
        out << secs / 60 << " min";
    }
    else if (secs < 7200)
    {
        out << "1 hr " << (secs - 3600) / 60 << " min";
    }
    else if (secs < 3600 * 24)
    {
        out << secs / 3600 << " hr";
    } else
    {
        out << secs / 3600 / 24 << " days";
    }

    return out.str();
}

bool GuildTaskMgr::SendItemAdvertisement(uint32 itemId, uint32 owner, uint32 guildId, uint32 validIn)
{
    Guild *guild = sGuildMgr.GetGuildById(guildId);
    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    Player* leader = sObjectMgr.GetPlayer(guild->GetLeaderGuid());

    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
    if (!proto)
        return false;

    ostringstream body;
    body << "Hello, " << player->GetName() << ",\n";
    body << "\n";
    body << "We are in a great need of " << proto->Name1 << ". If you could sell us ";
    uint32 count = GetTaskValue(owner, guildId, "itemCount");
    if (count > 1)
        body << "at least " << count << " of them ";
    else
        body << "some ";
    body << "we'd really appreciate that and pay a high price.\n";
    body << "The task will expire in " << formatTime(validIn) << "\n";
    body << "\n";
    body << "Best Regards,\n";
    body << guild->GetName() << "\n";
    body << leader->GetName() << "\n";

    ostringstream subject;
    subject << "Guild Task: " << proto->Name1;
    MailDraft(subject.str(), body.str()).SendMailTo(MailReceiver(player), MailSender(leader));

    return true;
}


bool GuildTaskMgr::SendKillAdvertisement(uint32 creatureId, uint32 owner, uint32 guildId, uint32 validIn)
{
    Guild *guild = sGuildMgr.GetGuildById(guildId);
    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    Player* leader = sObjectMgr.GetPlayer(guild->GetLeaderGuid());

    CreatureInfo const* proto = sObjectMgr.GetCreatureTemplate(creatureId);
    if (!proto)
        return false;

    ostringstream body;
    body << "Hello, " << player->GetName() << ",\n";
    body << "\n";
    body << "As you probably know " << proto->Name << " is wanted dead for the crimes it did against our guild. If you should kill it ";
    body << "we'd really appreciate that.\n";
    body << "The task will expire in " << formatTime(validIn) << "\n";
    body << "\n";
    body << "Best Regards,\n";
    body << guild->GetName() << "\n";
    body << leader->GetName() << "\n";

    ostringstream subject;
    subject << "Guild Task: " << proto->Name;
    MailDraft(subject.str(), body.str()).SendMailTo(MailReceiver(player), MailSender(leader));

    return true;
}

bool GuildTaskMgr::SendThanks(uint32 owner, uint32 guildId)
{
    Guild *guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
        return false;

    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    if (!player)
        return false;

    Player* leader = sObjectMgr.GetPlayer(guild->GetLeaderGuid());
    if (!leader)
        return false;

    uint32 itemTask = GetTaskValue(owner, guildId, "itemTask");
    if (itemTask)
    {
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemTask);
        if (!proto)
            return false;

        ostringstream body;
        body << "Hello, " << player->GetName() << ",\n";
        body << "\n";
        body << "One of our guild members wishes to thank you for the " << proto->Name1 << "! If we have another ";
        uint32 count = GetTaskValue(owner, guildId, "itemCount");
        body << count << " of them that would help us tremendously.\n";
        body << "\n";
        body << "Thanks again,\n";
        body << guild->GetName() << "\n";
        body << leader->GetName() << "\n";

        MailDraft("Thank You", body.str()).
                SetMoney(GetTaskValue(owner, guildId, "payment")).
                SendMailTo(MailReceiver(player), MailSender(leader));

        return true;
    }

    return false;
}

uint32 GuildTaskMgr::GetMaxItemTaskCount(uint32 itemId)
{
    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
    if (!proto)
        return 0;

    if (proto->Quality < ITEM_QUALITY_RARE && proto->Stackable && proto->GetMaxStackSize() > 1)
        return proto->GetMaxStackSize();
    else if (proto->Stackable && proto->GetMaxStackSize() > 1)
        return urand(1 + proto->GetMaxStackSize() / 4, proto->GetMaxStackSize());

    return 1;
}

bool GuildTaskMgr::IsGuildTaskItem(uint32 itemId, uint32 guildId)
{
    uint32 value = 0;

    QueryResult* results = CharacterDatabase.PQuery(
            "select `value`, `time`, validIn from ai_playerbot_guild_tasks where `value` = '%u' and guildid = '%u' and `type` = 'itemTask'",
            itemId, guildId);

    if (results)
    {
        Field* fields = results->Fetch();
        value = fields[0].GetUInt32();
        uint32 lastChangeTime = fields[1].GetUInt32();
        uint32 validIn = fields[2].GetUInt32();
        if ((time(0) - lastChangeTime) >= validIn)
            value = 0;

		delete results;
    }

    return value;
}

map<uint32,uint32> GuildTaskMgr::GetTaskValues(uint32 owner, string type, uint32 *validIn /* = NULL */)
{
    map<uint32,uint32> result;

    QueryResult* results = CharacterDatabase.PQuery(
            "select `value`, `time`, validIn, guildid from ai_playerbot_guild_tasks where owner = '%u' and `type` = '%s'",
            owner, type.c_str());

    if (!results)
        return result;

    do
    {
        Field* fields = results->Fetch();
        uint32 value = fields[0].GetUInt32();
        uint32 lastChangeTime = fields[1].GetUInt32();
        uint32 secs = fields[2].GetUInt32();
        uint32 guildId = fields[3].GetUInt32();
        if ((time(0) - lastChangeTime) >= secs)
            value = 0;

        result[guildId] = value;

    } while (results->NextRow());

	delete results;
	return result;
}

uint32 GuildTaskMgr::GetTaskValue(uint32 owner, uint32 guildId, string type, uint32 *validIn /* = NULL */)
{
    uint32 value = 0;

    QueryResult* results = CharacterDatabase.PQuery(
            "select `value`, `time`, validIn from ai_playerbot_guild_tasks where owner = '%u' and guildid = '%u' and `type` = '%s'",
            owner, guildId, type.c_str());

    if (results)
    {
        Field* fields = results->Fetch();
        value = fields[0].GetUInt32();
        uint32 lastChangeTime = fields[1].GetUInt32();
        uint32 secs = fields[2].GetUInt32();
        if ((time(0) - lastChangeTime) >= secs)
            value = 0;

        if (validIn) *validIn = secs;
    }

	delete results;
	return value;
}

uint32 GuildTaskMgr::SetTaskValue(uint32 owner, uint32 guildId, string type, uint32 value, uint32 validIn)
{
    CharacterDatabase.PExecute("delete from ai_playerbot_guild_tasks where owner = '%u' and guildid = '%u' and `type` = '%s'",
            owner, guildId, type.c_str());
    if (value)
    {
        CharacterDatabase.PExecute(
                "insert into ai_playerbot_guild_tasks (owner, guildid, `time`, validIn, `type`, `value`) values ('%u', '%u', '%u', '%u', '%s', '%u')",
                owner, guildId, (uint32)time(0), validIn, type.c_str(), value);
    }

    return value;
}

bool GuildTaskMgr::HandleConsoleCommand(ChatHandler* handler, char const* args)
{
    if (!sPlayerbotAIConfig.guildTaskEnabled)
    {
        sLog.outError( "Guild task system is currently disabled!");
        return false;
    }

    if (!args || !*args)
    {
        sLog.outError( "Usage: gtask stats/reset");
        return false;
    }

    string cmd = args;

    if (cmd == "reset")
    {
        CharacterDatabase.PExecute("delete from ai_playerbot_guild_tasks");
        sLog.outString("Guild tasks were reset for all players");
        return true;
    }

    if (cmd == "stats")
    {
        sLog.outString("Usage: gtask stats <player name>");
        return true;
    }

    if (cmd.find("stats ") != string::npos)
    {
        string charName = cmd.substr(cmd.find("stats ") + 6);
        ObjectGuid guid = sObjectMgr.GetPlayerGuidByName(charName);
        if (!guid)
        {
            sLog.outError( "Player %s not found", charName.c_str());
            return false;
        }

        uint32 owner = (uint32)guid.GetRawValue();

        QueryResult* result = CharacterDatabase.PQuery(
                "select `value`, `time`, validIn, guildid, `type` from ai_playerbot_guild_tasks where owner = '%u' order by guildid, `type`",
                owner);

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 value = fields[0].GetUInt32();
                uint32 lastChangeTime = fields[1].GetUInt32();
                uint32 validIn = fields[2].GetUInt32();
                if ((time(0) - lastChangeTime) >= validIn)
                    value = 0;
                uint32 guildId = fields[3].GetUInt32();
                string type = fields[4].GetString();

                Guild *guild = sGuildMgr.GetGuildById(guildId);
                if (!guild)
                    continue;

                ostringstream name;
                name << value;
                if (type == "killTask")
                {
                    CreatureInfo const* proto = sObjectMgr.GetCreatureTemplate(value);
                    string rank = proto->Rank == CREATURE_ELITE_RARE ? "rare" : "elite";
                    if (proto) name << " (" << proto->Name << "," << rank << ")";
                }
                else if (type == "itemTask")
                {
                    ItemPrototype const* proto = sObjectMgr.GetItemPrototype(value);
                    string rank = proto->Quality == ITEM_QUALITY_UNCOMMON ? "uncommon" : "rare";
                    if (proto) name << " (" << proto->Name1 << "," << rank << ")";
                }

                sLog.outString("Player '%s' Guild '%s' %s=%s (%u secs)",
                        charName.c_str(), guild->GetName().c_str(),
                        type.c_str(), name.str().c_str(), validIn);

            } while (result->NextRow());

            Field* fields = result->Fetch();
			delete result;
        }

        return true;
    }

    if (cmd == "reward")
    {
        sLog.outString("Usage: gtask reward <player name>");
        return true;
    }

    if (cmd.find("reward ") != string::npos)
    {
        string charName = cmd.substr(cmd.find("reward ") + 7);
        ObjectGuid guid = sObjectMgr.GetPlayerGuidByName(charName);
        if (!guid)
        {
            sLog.outError( "Player %s not found", charName.c_str());
            return false;
        }

        uint32 owner = (uint32)guid.GetRawValue();
        QueryResult* result = CharacterDatabase.PQuery(
                "select distinct guildid from ai_playerbot_guild_tasks where owner = '%u'",
                owner);

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 guildId = fields[0].GetUInt32();
                Guild *guild = sGuildMgr.GetGuildById(guildId);
                if (!guild)
                    continue;

                sGuildTaskMgr.Reward(owner, guildId);
            } while (result->NextRow());

            Field* fields = result->Fetch();
			delete result;
            return true;
        }
    }

    return false;
}

void GuildTaskMgr::CheckItemTask(uint32 itemId, uint32 obtained, Player* ownerPlayer, Player* bot, bool byMail)
{
    uint32 guildId = bot->GetGuildId();
    if (!guildId)
        return;

    uint32 owner = (uint32)ownerPlayer->GetGUID();
	Guild *guild = sGuildMgr.GetGuildById(ownerPlayer->GetGuildId());

    sLog.outDebug("%s / %s: checking guild task",
			guild->GetName().c_str(), ownerPlayer->GetName());

    uint32 itemTask = GetTaskValue(owner, guildId, "itemTask");
    if (itemTask != itemId)
    {
        sLog.outDebug("%s / %s: item %u is not guild task item (%u)",
				guild->GetName().c_str(), ownerPlayer->GetName(),
                itemId, itemTask);
        return;
    }

    if (byMail)
    {
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemId);
        if (!proto)
            return;

        uint32 money = GetTaskValue(owner, guildId, "payment");
        SetTaskValue(owner, guildId, "payment", money + auctionbot.GetBuyPrice(proto) * obtained,
                sPlayerbotAIConfig.maxGuildTaskRewardTime);
    }

    uint32 count = GetTaskValue(owner, guildId, "itemCount");
    if (obtained >= count)
    {
        sLog.outDebug("%s / %s: guild task complete",
				guild->GetName().c_str(), ownerPlayer->GetName());
        SetTaskValue(owner, guildId, "reward", 1,
                urand(sPlayerbotAIConfig.minGuildTaskRewardTime, sPlayerbotAIConfig.maxGuildTaskRewardTime));
        ChatHandler(ownerPlayer->GetSession()).PSendSysMessage("You have completed a guild task");
    }
    else
    {
        sLog.outDebug("%s / %s: guild task progress",
				guild->GetName().c_str(), ownerPlayer->GetName());
        SetTaskValue(owner, guildId, "itemCount", count - obtained, sPlayerbotAIConfig.maxGuildTaskChangeTime);
        SetTaskValue(owner, guildId, "thanks", 1,
                urand(sPlayerbotAIConfig.minGuildTaskRewardTime, sPlayerbotAIConfig.maxGuildTaskRewardTime));
    }
}

bool GuildTaskMgr::Reward(uint32 owner, uint32 guildId)
{
    Guild *guild = sGuildMgr.GetGuildById(guildId);
    if (!guild)
        return false;

    Player* player = sObjectMgr.GetPlayer((uint64)owner);
    if (!player)
        return false;

    Player* leader = sObjectMgr.GetPlayer(guild->GetLeaderGuid());
    if (!leader)
        return false;

    uint32 itemTask = GetTaskValue(owner, guildId, "itemTask");
    uint32 killTask = GetTaskValue(owner, guildId, "killTask");
    if (!itemTask && !killTask)
        return false;

    ostringstream body;
    body << "Hello, " << player->GetName() << ",\n";
    body << "\n";

    RandomItemType rewardType;
    if (itemTask)
    {
        ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itemTask);
        if (!proto)
            return false;

        body << "We wish to thank you for the " << proto->Name1 << " you provided so kindly. We really appreciate this and may this small gift bring you our thanks!\n";
        body << "\n";
        body << "Many thanks,\n";
        body << guild->GetName() << "\n";
        body << leader->GetName() << "\n";
        rewardType = RANDOM_ITEM_GUILD_TASK_REWARD_EQUIP;
    }
    else if (killTask)
    {
        CreatureInfo const* proto = sObjectMgr.GetCreatureTemplate(killTask);
        if (!proto)
            return false;

        body << "We wish to thank you for the " << proto->Name << " you've killed recently. We really appreciate this and may this small gift bring you our thanks!\n";
        body << "\n";
        body << "Many thanks,\n";
        body << guild->GetName() << "\n";
        body << leader->GetName() << "\n";
        rewardType = RANDOM_ITEM_GUILD_TASK_REWARD_TRADE;
    }

    MailDraft draft("Thank You", body.str());

    uint32 itemId = sRandomItemMgr.GetRandomItem(rewardType);
    if (itemId)
    {
        Item* item = Item::CreateItem(itemId, 1, leader);
        item->SaveToDB();
        draft.AddItem(item);
    }

    draft.SetMoney(GetTaskValue(owner, guildId, "payment")).SendMailTo(MailReceiver(player), MailSender(leader));

    SetTaskValue(owner, guildId, "activeTask", 0, 0);
    return true;
}

void GuildTaskMgr::CheckKillTask(Player* player, Unit* victim)
{
    uint32 owner = player->GetGUID();
    Creature* creature = victim->ToCreature();
    if (!creature)
        return;

    map<uint32,uint32> tasks = GetTaskValues(owner, "killTask");
    for (map<uint32,uint32>::iterator i = tasks.begin(); i != tasks.end(); ++i)
    {
        uint32 guildId = i->first;
        uint32 value = i->second;
        Guild* guild = sGuildMgr.GetGuildById(guildId);

        if (value != creature->GetCreatureInfo()->Entry)
            continue;

        sLog.outDebug("%s / %s: guild task complete",
                guild->GetName().c_str(), player->GetName());
        SetTaskValue(owner, guildId, "reward", 1,
                urand(sPlayerbotAIConfig.minGuildTaskRewardTime, sPlayerbotAIConfig.maxGuildTaskRewardTime));
        ChatHandler(player->GetSession()).PSendSysMessage("You have completed a guild task");
    }
}
