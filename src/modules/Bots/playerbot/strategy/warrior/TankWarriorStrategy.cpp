#include "botpch.h"
#include "../../playerbot.h"
#include "WarriorMultipliers.h"
#include "TankWarriorStrategy.h"

using namespace ai;

class TankWarriorStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    TankWarriorStrategyActionNodeFactory()
    {
        creators["melee"] = &melee;
        creators["shield wall"] = &shield_wall;
     // creators["rend"] = &rend;
        creators["revenge"] = &revenge;
		creators["shield slam"] = &shield_slam;
	//	creators["thunder clap"] = &thunder_clap;
		creators["shield block"] = &shield_block;
        creators["sunder armor"] = &sunder_armor;
        creators["taunt"] = &taunt;
    }
private:
    static ActionNode* melee(PlayerbotAI* ai)
    {
        return new ActionNode ("melee",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* shield_wall(PlayerbotAI* ai)
    {
        return new ActionNode ("shield wall",
			/*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("shield block"), NULL),
            /*C*/ NULL);
    }
    static ActionNode* shield_slam(PlayerbotAI* ai)
    {
        return new ActionNode ("shield slam",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
    static ActionNode* revenge(PlayerbotAI* ai)
    {
        return new ActionNode ("revenge",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NULL,
            /*C*/ NULL);
    }
   static ActionNode* sunder_armor(PlayerbotAI* ai)
    {
        return new ActionNode ("sunder armor",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("revenge"), NULL),
			/*C*/ NULL);
    }
   static ActionNode* shield_block(PlayerbotAI* ai)
    {
        return new ActionNode ("shield block",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NULL,
            /*C*/ NextAction::array(0, new NextAction("revenge"), NULL));
    }
    static ActionNode* taunt(PlayerbotAI* ai)
    {
        return new ActionNode ("taunt",
            /*P*/ NextAction::array(0, new NextAction("defensive stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("sunder armor"), NULL),
            /*C*/ NULL);
    }
//	static ActionNode* thunder_clap(PlayerbotAI* ai)
//	{
//		return new ActionNode("thunder clap",
//			/*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
//			/*A*/ NULL,
//			/*C*/ NextAction::array(0, new NextAction("defensive stance"), NULL));
//	}
};

TankWarriorStrategy::TankWarriorStrategy(PlayerbotAI* ai) : GenericWarriorStrategy(ai)
{
    actionNodeFactories.Add(new TankWarriorStrategyActionNodeFactory());
}

NextAction** TankWarriorStrategy::getDefaultActions()
{
	return NextAction::array(0, 
		new NextAction("revenge", ACTION_NORMAL + 6),
		new NextAction("shield slam", ACTION_NORMAL + 5),
		new NextAction("shield block", ACTION_NORMAL + 4),
		new NextAction("sunder armor", ACTION_NORMAL + 3),
		new NextAction("heroic strike", ACTION_NORMAL + 2),
		new NextAction("melee", ACTION_NORMAL),
		NULL);

}

void TankWarriorStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarriorStrategy::InitTriggers(triggers);


/*    triggers.push_back(new TriggerNode(
        "high rage available",
        NextAction::array(0, new NextAction("heroic strike", ACTION_NORMAL + 1), new NextAction("revenge", ACTION_NORMAL + 1), NULL)));*/

    triggers.push_back(new TriggerNode(
        "disarm",
        NextAction::array(0, new NextAction("disarm", ACTION_NORMAL), NULL)));

    triggers.push_back(new TriggerNode(
        "lose aggro",
        NextAction::array(0, new NextAction("taunt", ACTION_HIGH + 9), NULL)));

    triggers.push_back(new TriggerNode(
        "low health",
        NextAction::array(0, new NextAction("shield wall", ACTION_MEDIUM_HEAL), NULL)));

	triggers.push_back(new TriggerNode(
		"critical health",
		NextAction::array(0, new NextAction("last stand", ACTION_EMERGENCY + 3), NULL)));

	triggers.push_back(new TriggerNode(
		"light aoe",
		NextAction::array(0, new NextAction("cleave", ACTION_NORMAL + 2), NULL)));

	triggers.push_back(new TriggerNode(
        "medium aoe",
        NextAction::array(0, new NextAction("demoralizing shout", ACTION_HIGH + 1),  new NextAction("cleave", ACTION_HIGH + 1), NULL)));

    triggers.push_back(new TriggerNode(
        "high aoe",
        NextAction::array(0, new NextAction("challenging shout", ACTION_HIGH + 3), new NextAction("cleave", ACTION_HIGH + 1), NULL)));

	triggers.push_back(new TriggerNode(
		"concussion blow",
		NextAction::array(0, new NextAction("concussion blow", ACTION_INTERRUPT), NULL)));

    triggers.push_back(new TriggerNode(
        "revenge",
        NextAction::array(0, new NextAction("revenge", ACTION_NORMAL + 6), NULL)));

	triggers.push_back(new TriggerNode(
		"has aggro",
		NextAction::array(0, new NextAction("shield block", ACTION_NORMAL + 4), NULL)));

	triggers.push_back(new TriggerNode(
		"sunder armor",
		NextAction::array(0, new NextAction("sunder armor", ACTION_NORMAL + 2), NULL)));
}
