#include "botpch.h"
#include "../../playerbot.h"
#include "WarriorMultipliers.h"
#include "ArmsWarriorStrategy.h"

using namespace ai;                          

class ArmsWarriorStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
	ArmsWarriorStrategyActionNodeFactory()
    {
		creators["overpower"] = &overpower;
		creators["melee"] = &melee;
		creators["charge"] = &charge;
		creators["mortal strike"] = &mortal_strike;
		creators["whirlwind"] = &whirlwind;
		creators["death wish"] = &death_wish;
		creators["execute"] = &execute;
    }
private:
    static ActionNode* overpower(PlayerbotAI* ai)
    {
        return new ActionNode ("overpower",
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* melee(PlayerbotAI* ai)
    {
        return new ActionNode ("melee",
            /*P*/ NextAction::array(0, new NextAction("reach melee"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* charge(PlayerbotAI* ai)
    {
        return new ActionNode ("charge",
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("reach melee"), NULL),
            /*C*/ NULL);
    }
	static ActionNode* mortal_strike(PlayerbotAI* ai)
	{
		return new ActionNode("mortal strike",
			/*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
			/*A*/ NULL,
			/*C*/ NULL);
    }
	static ActionNode* whirlwind(PlayerbotAI* ai)
	{
		return new ActionNode("whirlwind",
			/*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
			/*A*/ NULL,
			/*C*/ NULL);
    }
    static ActionNode* death_wish(PlayerbotAI* ai)
    {
        return new ActionNode ("death wish",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("berserker rage"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* execute(PlayerbotAI* ai)
    {
        return new ActionNode ("execute",
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("heroic strike"), NULL),
            /*C*/ NULL);
    }
};

ArmsWarriorStrategy::ArmsWarriorStrategy(PlayerbotAI* ai) : GenericWarriorStrategy(ai)
{
    actionNodeFactories.Add(new ArmsWarriorStrategyActionNodeFactory());
}

NextAction** ArmsWarriorStrategy::getDefaultActions()
{
	return NextAction::array(0, new NextAction("mortal strike", ACTION_NORMAL + 8), NULL);
}

void ArmsWarriorStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarriorStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        "enemy out of melee",
        NextAction::array(0, new NextAction("charge", ACTION_NORMAL + 9), NULL)));

	triggers.push_back(new TriggerNode(
		"mortal strike",
		NextAction::array(0, new NextAction("mortal strike", ACTION_NORMAL + 8), NULL)));

	triggers.push_back(new TriggerNode(
		"overpower",
		NextAction::array(0, new NextAction("overpower", ACTION_NORMAL + 8), NULL)));

	triggers.push_back(new TriggerNode(
		"whirlwind",
		NextAction::array(0, new NextAction("whirlwind", ACTION_NORMAL + 8), NULL)));

    triggers.push_back(new TriggerNode(
        "target critical health",
        NextAction::array(0, new NextAction("execute", ACTION_HIGH + 4), NULL)));

	triggers.push_back(new TriggerNode(
		"hamstring",
		NextAction::array(0, new NextAction("hamstring", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "death wish",
        NextAction::array(0, new NextAction("death wish", ACTION_HIGH + 2), NULL)));
}


void ArmsWarrirorAoeStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    triggers.push_back(new TriggerNode(
        "light aoe",
		NextAction::array(0, new NextAction("thunder clap", ACTION_HIGH + 3),
			new NextAction("demoralizing shout", ACTION_HIGH + 2),
			new NextAction("cleave", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("sweeping strikes", ACTION_HIGH + 3), new NextAction("recklessness", ACTION_HIGH + 2), NULL)));
}
