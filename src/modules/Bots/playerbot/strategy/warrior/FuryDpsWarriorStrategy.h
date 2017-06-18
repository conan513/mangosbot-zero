#pragma once

#include "GenericWarriorStrategy.h"

namespace ai
{
	class FuryDpsWarriorStrategy : public GenericWarriorStrategy
	{
	public:
		FuryDpsWarriorStrategy(PlayerbotAI* ai);

	public:
		virtual void InitTriggers(std::list<TriggerNode*> &triggers);
		virtual string getName() { return "fury dps"; }
		virtual NextAction** getDefaultActions();
		virtual int GetType() { return STRATEGY_TYPE_COMBAT | STRATEGY_TYPE_DPS | STRATEGY_TYPE_MELEE; }
	};

	class FuryDpsWarriorAoeStrategy : public CombatStrategy
	{
	public:
		FuryDpsWarriorAoeStrategy(PlayerbotAI* ai) : CombatStrategy(ai) {}

	public:
		virtual void InitTriggers(std::list<TriggerNode*> &triggers);
		virtual string getName() { return "fury aoe"; }
	};
}