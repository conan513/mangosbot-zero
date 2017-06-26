#pragma once

#include "../Action.h"
#include "InventoryAction.h"

namespace ai
{
    class MailAction : public InventoryAction
    {
    public:
        MailAction(PlayerbotAI* ai) : InventoryAction(ai, "mail") {}

        virtual bool Execute(Event event);
    };

}
