#pragma once

#include "../Strategy.h"
#include "../generic/MeleeCombatStrategy.h"

namespace ai
{
	class CombatDpsRogueStrategy : public MeleeCombatStrategy
	{
	public:
		CombatDpsRogueStrategy(PlayerbotAI* ai);

	public:
		virtual void InitTriggers(std::list<TriggerNode*> &triggers);
		virtual string getName() { return "combat dps"; }
		virtual NextAction** getDefaultActions();
	};
}

