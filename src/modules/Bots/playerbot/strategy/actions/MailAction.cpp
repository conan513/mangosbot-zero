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
    bool mailboxFound = false;
    for (list<ObjectGuid>::iterator i = gos.begin(); i != gos.end(); ++i)
    {
        GameObject* go = ai->GetGameObject(*i);
        if (go && go->GetGoType() == GAMEOBJECT_TYPE_MAILBOX)
        {
            mailboxFound = true;
            break;
        }
    }

    if (!mailboxFound)
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
                out << " " << ChatHelper::formatMoney(mail->money);
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

    return false;
}
