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
        ai->TellMaster("--- Mailbox ---");
        time_t cur_time = time(0);
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;
            int days = (cur_time - mail->deliver_time) / 3600 / 24;
            ostringstream out;
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
    else if (text == "take")
    {
        time_t cur_time = time(0);
        for (PlayerMails::iterator itr = bot->GetMailBegin(); itr != bot->GetMailEnd(); ++itr)
        {
            if ((*itr)->state == MAIL_STATE_DELETED || cur_time < (*itr)->deliver_time)
                continue;

            Mail *mail = *itr;
            if (mail->money)
            {
                ostringstream out;
                out << mail->subject << ", |cffffff00" << ChatHelper::formatMoney(mail->money);
                ai->TellMaster(out.str());

                WorldPacket packet;
                packet << mailbox;
                packet << mail->messageID;
                bot->GetSession()->HandleMailTakeMoney(packet);
                bot->RemoveMail(mail->messageID);
                continue;
            }

            if (!mail->items.empty())
            {
                for (MailItemInfoVec::iterator i = mail->items.begin(); i != mail->items.end(); ++i)
                {
                    ItemPrototype const *proto = sObjectMgr.GetItemPrototype(i->item_template);
                    if (proto)
                    {
                        ostringstream out;
                        out << mail->subject << ", " << ChatHelper::formatItem(proto);
                        ai->TellMaster(out.str());
                    }
                }

                WorldPacket packet;
                packet << mailbox;
                packet << mail->messageID;
                bot->GetSession()->HandleMailTakeItem(packet);
                bot->RemoveMail(mail->messageID);
                continue;
            }
        }
    }
    else
        ai->TellMaster("whisper 'mail ?' to query mailbox, 'mail take' to take all items");

    return true;
}
