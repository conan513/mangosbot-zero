#pragma once

#include "../Strategy.h"
#include "../generic/MeleeCombatStrategy.h"

namespace ai
{
	class SubDpsRogueStrategy : public MeleeCombatStrategy
	{
	public:
		SubDpsRogueStrategy(PlayerbotAI* ai);

	public:
		virtual void InitTriggers(std::list<TriggerNode*> &triggers);
		virtual string getName() { return "sub dps"; }
		virtual NextAction** getDefaultActions();
	};
}

