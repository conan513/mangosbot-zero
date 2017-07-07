#include "botpch.h"
#include "Mail.h"
#include "../../playerbot.h"
#include "MailAction.h"
#include "../../PlayerbotAIConfig.h"

using namespace ai;

bool MailAction::Execute(Event event)
{
    Player* master = GetMaster();
    if (!master)
        return false;

    list<ObjectGuid> gos = *context->GetValue<list<ObjectGuid> >("nearest game objects");
    ObjectGuid mailbox;
    for (list<ObjectGuid>::iterator i = gos.begin(); i != gos.end(); ++i)
    {
        GameObject* go = ai->GetGameObject(*i);
        if (go && go->GetGoType() == GAMEOBJECT_TYPE_MAILBOX)
        {
            mailbox = go->GetObjectGuid();
            break;
        }
    }

    if (!mailbox)
    {
        ai->TellMaster("There is no mailbox nearby");
        return false;
    }

    string text = event.getParam();
    if (text == "?")
    {
        ai->TellMaster("=== Mailbox ===");
        time_t cur_time = time(0);
        int index = 1;
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;
            int days = (cur_time - mail->deliver_time) / 3600 / 24;
            ostringstream out;
            out << "#" << index++ << " ";
            out << "|cffffffff" << mail->subject;
            if (mail->money)
                out << ", |cffffff00" << ChatHelper::formatMoney(mail->money);
            if (mail->has_items)
            {
                for (MailItemInfoVec::iterator i = mail->items.begin(); i != mail->items.end(); ++i)
                {
                    ItemPrototype const *proto = sObjectMgr.GetItemPrototype(i->item_template);
                    if (proto)
                        out << ", " << ChatHelper::formatItem(proto);
                }
            }
            out  << ", |cff00ff00" << days << " day(s)";
            ai->TellMaster(out.str());
        }
        return true;
    }
    else if (text.find("take ") != string::npos)
    {
        time_t cur_time = time(0);
        int index = 1;
        vector<string> ids = split(text.substr(5), ',');
        vector<string>::iterator i = ids.begin();
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;

            if (index++ != atoi(i->c_str()) && *i != "*")
                continue;

            uint32 totalused = 0, total = 16;
            for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
            {
                if (bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    totalused++;
            }
            uint32 totalfree = 16 - totalused;
            for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
            {
                if (const Bag* const pBag = (Bag*) bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag))
                {
                    ItemPrototype const* pBagProto = pBag->GetProto();
                    if (pBagProto->Class == ITEM_CLASS_CONTAINER && pBagProto->SubClass == ITEM_SUBCLASS_CONTAINER)
                        totalfree += pBag->GetFreeSlots();
                }

            }

            if (totalfree < 2)
            {
                ai->TellMaster("Not enough bag space");
                return false;
            }

            if (mail->money)
            {
                ostringstream out;
                out << mail->subject << ", |cffffff00" << ChatHelper::formatMoney(mail->money) << "|cff00ff00 processed";
                ai->TellMaster(out.str());

                WorldPacket packet;
                packet << mailbox;
                packet << mail->messageID;
                bot->GetSession()->HandleMailTakeMoney(packet);
                RemoveMail(mail->messageID, mailbox);
            }
            else if (!mail->items.empty())
            {
                for (MailItemInfoVec::iterator i = mail->items.begin(); i != mail->items.end(); ++i)
                {
                    ItemPrototype const *proto = sObjectMgr.GetItemPrototype(i->item_template);
                    if (proto)
                    {
                        ostringstream out;
                        out << mail->subject << ", " << ChatHelper::formatItem(proto) << "|cff00ff00 processed";
                        ai->TellMaster(out.str());
                    }
                }

                WorldPacket packet;
                packet << mailbox;
                packet << mail->messageID;
                bot->GetSession()->HandleMailTakeItem(packet);
                RemoveMail(mail->messageID, mailbox);
            }

            if (*i != "*")
            {
                ++i;
                if (i == ids.end())
                    break;
            }
        }
    }
    else if (text.find("delete ") != string::npos)
    {
        time_t cur_time = time(0);
        int index = 1;
        vector<string> ids = split(text.substr(7), ',');
        vector<string>::iterator i = ids.begin();
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;
            if (index++ == atoi(i->c_str()))
            {
                ostringstream out;
                out << "|cffffffff" << mail->subject << "|cffff0000 deleted";
                ai->TellMaster(out.str());
                RemoveMail(mail->messageID, mailbox);
                ++i;
                if (i == ids.end())
                    break;
            }
        }
    }
    else if (text.find("read ") != string::npos)
    {
        time_t cur_time = time(0);
        int index = 1;
        vector<string> ids = split(text.substr(5), ',');
        vector<string>::iterator i = ids.begin();
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;
            if (index++ == atoi(i->c_str()))
            {
                ostringstream out, body;
                out << "|cffffffff" << mail->subject;
                ai->TellMaster(out.str());
                if (mail->itemTextId)
                {
                    body << "\n" << sObjectMgr.GetItemText(mail->itemTextId);
                    ai->TellMaster(body.str());
                }
                ++i;
                if (i == ids.end())
                    break;
            }
        }
    }
    else
        ai->TellMaster("whisper 'mail ?' to query mailbox, 'mail take' to take all items, 'mail delete N' to delete mail #N");

    return true;
}

void MailAction::RemoveMail(uint32 id, ObjectGuid mailbox)
{
    WorldPacket packet;
    packet << mailbox;
    packet << id;
    bot->GetSession()->HandleMailDelete(packet);
}
