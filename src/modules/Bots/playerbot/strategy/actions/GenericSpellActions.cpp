#include "botpch.h"
#include "../../playerbot.h"
#include "GenericActions.h"
#include "PlayerbotFactory.h"
#include "PlayerbotAI.h"


using namespace ai;


// Return a poison Item based
Item* CastSpellAction::FindPoison() const
{
	// Note: in Classic, wound poison and crippling poison share the same display ID
	// If bot has both in his/her inventory, the first one picked will be used, be it a wound poison or not
	static const uint32 uPriorizedPoisonIds[3] =
	{
		INSTANT_POISON_DISPLAYID, WOUND_POISON_DISPLAYID, DEADLY_POISON_DISPLAYID
	};

	Item* poison;
	for (uint8 i = 0; i < countof(uPriorizedPoisonIds); ++i)
	{
		poison = FindConsumable(uPriorizedPoisonIds[i]);
		if (poison)
			return poison;
	}
	return nullptr;
}
Item* PlayerbotAI::FindConsumable(uint32 displayId) const
{
	// list out items in main backpack
	for (uint8 slot = INVENTORY_SLOT_ITEM_START; slot < INVENTORY_SLOT_ITEM_END; slot++)
	{
		Item* const pItem = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
		if (pItem)
		{
			const ItemPrototype* const pItemProto = pItem->GetProto();

			if (!pItemProto || bot->CanUseItem(pItemProto) != EQUIP_ERR_OK)
				continue;

			if ((pItemProto->Class == ITEM_CLASS_CONSUMABLE || pItemProto->Class == ITEM_SUBCLASS_BANDAGE) && pItemProto->DisplayInfoID == displayId)
				return pItem;
		}
	}
	// list out items in other removable backpacks
	for (uint8 bag = INVENTORY_SLOT_BAG_START; bag < INVENTORY_SLOT_BAG_END; ++bag)
	{
		const Bag* const pBag = (Bag *)bot->GetItemByPos(INVENTORY_SLOT_BAG_0, bag);
		if (pBag)
			for (uint8 slot = 0; slot < pBag->GetBagSize(); ++slot)
			{
				Item* const pItem = bot->GetItemByPos(bag, slot);
				if (pItem)
				{
					const ItemPrototype* const pItemProto = pItem->GetProto();

					if (!pItemProto || bot->CanUseItem(pItemProto) != EQUIP_ERR_OK)
						continue;

					if ((pItemProto->Class == ITEM_CLASS_CONSUMABLE || pItemProto->Class == ITEM_SUBCLASS_BANDAGE) && pItemProto->DisplayInfoID == displayId)
						return pItem;
				}
			}
	}
	return nullptr;
}
bool CastSpellAction::isPossible()
{
	return ai->CanCastSpell(spell, GetTarget());
}

bool CastSpellAction::isUseful()
{
	return GetTarget() && AI_VALUE2(bool, "spell cast useful", spell);
}

bool CastAuraSpellAction::isUseful()
{
	return CastSpellAction::isUseful() && !ai->HasAura(spell, GetTarget());
}

bool CastEnchantItemAction::isPossible()
{
	if (!CastSpellAction::isPossible())
		return false;

	uint32 spellId = AI_VALUE2(uint32, "spell id", spell);
	return spellId && AI_VALUE2(Item*, "item for spell", spellId);
}

bool CastHealingSpellAction::isUseful()
{
	return CastAuraSpellAction::isUseful() && AI_VALUE2(uint8, "health", GetTargetName()) < (100 - estAmount);
}

bool CastAoeHealSpellAction::isUseful()
{
	return CastSpellAction::isUseful() && AI_VALUE2(uint8, "aoe heal", "medium") > 0;
}


Value<Unit*>* CurePartyMemberAction::GetTargetValue()
{
	return context->GetValue<Unit*>("party member to dispel", dispelType);
}

Value<Unit*>* BuffOnPartyAction::GetTargetValue()
{
	return context->GetValue<Unit*>("party member without aura", spell);
}

bool CastSpellAction::Execute(Event event)
{
	if (spell=="poisoning")

	
	// remove stealth
	if (bot->HasAura(SPELL_AURA_MOD_STEALTH))
		bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

	// Search and apply poisons to weapons, if no poison found, try to apply a sharpening/weight stone
	// Mainhand ...
	Item * poison, *stone, *weapon;
	weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
	if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
	{
		poison = FindPoison();
		if (poison)
		{
			bot->ApplyEnchantment(poison, EQUIPMENT_SLOT_MAINHAND);
			ai->SetNextCheckDelay(5);
		}
		
	}
	//... and offhand
	weapon = bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
	if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
	{
		poison = FindPoison();
		if (poison)
		{
			bot->ApplyEnchantment(poison, EQUIPMENT_SLOT_OFFHAND);
			ai->SetNextCheckDelay(5);
		}
		
	}


};