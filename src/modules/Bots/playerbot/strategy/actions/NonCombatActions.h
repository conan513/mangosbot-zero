#pragma once

#include "../Action.h"
#include "UseItemAction.h"
#include "../../PlayerbotAIConfig.h"
#include "../../PlayerbotAI.h"

namespace ai
{
    class DrinkAction : public UseItemAction
    {
    public:
        DrinkAction(PlayerbotAI* ai) : UseItemAction(ai, "drink") {}

        virtual bool Execute(Event event)
        {
            if (bot->IsInCombat())
                return false;

            return UseItemAction::Execute(event);
        }

        virtual bool isUseful()
        {
            return UseItemAction::isUseful() && AI_VALUE2(uint8, "mana", "self target") < sPlayerbotAIConfig.lowMana;
        }
    };

    class EatAction : public UseItemAction
    {
    public:
        EatAction(PlayerbotAI* ai) : UseItemAction(ai, "food") {}

        virtual bool Execute(Event event)
        {
            if (bot->IsInCombat())
                return false;

            return UseItemAction::Execute(event);
        }

        virtual bool isUseful()
        {
            return UseItemAction::isUseful() && AI_VALUE2(uint8, "health", "self target") < sPlayerbotAIConfig.lowHealth;
        }
    };

	class PoisonAction : public UseItemAction
	{
	public:
		PoisonAction(PlayerbotAI* ai) : UseItemAction(ai, "poison") {}

		virtual bool Execute(Event event)
		{
			if (bot->IsInCombat())
				return false;

			// remove stealth
			if (bot->HasAura(SPELL_AURA_MOD_STEALTH))
				bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

			// hp check
			if (bot->getStandState() != UNIT_STAND_STATE_STAND)
				bot->SetStandState(UNIT_STAND_STATE_STAND);


			// Search and apply poisons to weapons
			// Mainhand ...
			Item * poison, *weapon;
			weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
			if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
			{
				poison = ai->FindConsumable(INSTANT_POISON_DISPLAYID);
				if (!poison)
					poison = ai->FindConsumable(DEADLY_POISON_DISPLAYID);
				if (!poison)
					poison = ai->FindConsumable(WOUND_POISON_DISPLAYID);
				if (poison)
				{
					ai->UseItem(poison, EQUIPMENT_SLOT_MAINHAND);
					ai->SetNextCheckDelay(5);
				}
			}
			//... and offhand
			weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
			if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
			{
				poison = ai->FindConsumable(DEADLY_POISON_DISPLAYID);
				if (!poison)
					poison = ai->FindConsumable(WOUND_POISON_DISPLAYID);
				if (!poison)
					poison = ai->FindConsumable(INSTANT_POISON_DISPLAYID);
				if (poison)
				{
					ai->UseItem(poison, EQUIPMENT_SLOT_OFFHAND);
					ai->SetNextCheckDelay(5);
				}
			}

		}

	};

}