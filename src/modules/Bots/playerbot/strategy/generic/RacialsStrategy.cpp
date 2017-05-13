#include "botpch.h"
#include "../../playerbot.h"
#include "RacialsStrategy.h"

using namespace ai;


class RacialsStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    RacialsStrategyActionNodeFactory()
    {
        creators["berserking"] = &berserking;			//boost? or often?
		creators["blood fury"] = &blood_fury;			//boost? or often?
		creators["cannibalize"] = &cannibalize;			//loot
		creators["escape artist"] = &escape_artist;		//cc need trigger
		creators["shadowmeld"] = &shadowmeld;
		creators["stoneform"] = &stoneform;				//ch
		creators["war stomp"] = &war_stomp;				//cc need trigger
		creators["will of the forsaken"] = &will_of_the_forsaken;	//cc need trigger
		creators["feedback"] = &feedback;						//cc need trigger
		creators["desperate prayer"] = &desperate_prayer;		//ch	
		creators["fear ward"] = &fear_ward;						//cc need trigger
		creators["elune's grace"] = &elunes_grace;				//ch
		creators["starshards"] = &starshards;					//boost? or often?
		creators["touch of weakness"] = &touch_of_weakness;		//boost? or often?
		creators["devouring plague"] = &devouring_plague;		//boost? or often?
		creators["hex of weakness"] = &hex_of_weakness;			//boost? or often?
		creators["shadowguard"] = &shadowguard;					//ch
    }
private:
    static ActionNode* berserking(PlayerbotAI* ai)
    {
        return new ActionNode ("berserking",  
            /*P*/ NULL,
            /*A*/ NULL,
            /*C*/ NULL);
    }
	static ActionNode* blood_fury(PlayerbotAI* ai)
	{
		return new ActionNode("blood fury",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* cannibalize(PlayerbotAI* ai)
	{
		return new ActionNode("cannibalize",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* escape_artist(PlayerbotAI* ai)
	{
		return new ActionNode("escape artist",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* shadowmeld(PlayerbotAI* ai)
	{
		return new ActionNode("shadowmeld",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* stoneform(PlayerbotAI* ai)
	{
		return new ActionNode("stoneform",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* war_stomp(PlayerbotAI* ai)
	{
		return new ActionNode("war stomp",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* will_of_the_forsaken(PlayerbotAI* ai)
	{
		return new ActionNode("will of the forsaken",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* feedback(PlayerbotAI* ai)
	{
		return new ActionNode("feedback",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* desperate_prayer(PlayerbotAI* ai)
	{
		return new ActionNode("desperate prayer",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* fear_ward(PlayerbotAI* ai)
	{
		return new ActionNode("fear ward",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* elunes_grace(PlayerbotAI* ai)
	{
		return new ActionNode("elune's grace",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* starshards(PlayerbotAI* ai)
	{
		return new ActionNode("starshards",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* touch_of_weakness(PlayerbotAI* ai)
	{
		return new ActionNode("touch of weakness",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* devouring_plague(PlayerbotAI* ai)
	{
		return new ActionNode("devouring plague",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* hex_of_weakness(PlayerbotAI* ai)
	{
		return new ActionNode("hex of weakness",
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
	static ActionNode* shadowguard(PlayerbotAI* ai)
	{
		return new ActionNode("shadowguard",  
			/*P*/ NULL,
			/*A*/ NULL,
			/*C*/ NULL);
	}
};

void RacialsStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
	triggers.push_back(new TriggerNode(
		"critical health", 
		NextAction::array(0, new NextAction("stoneform", ACTION_EMERGENCY + 6),
			new NextAction("desperate prayer", ACTION_EMERGENCY + 6),
			new NextAction("elune's grace", ACTION_EMERGENCY + 6), NULL)));

    triggers.push_back(new TriggerNode(
        "loot available", 
        NextAction::array(0, new NextAction("cannibalize", ACTION_HIGH + 6), NULL)));
	
	triggers.push_back(new TriggerNode(
		"often",
		NextAction::array(0, new NextAction("berserking", ACTION_HIGH + 6),
			new NextAction("blood fury", ACTION_HIGH + 6),
			new NextAction("starshards", ACTION_HIGH + 6),
			new NextAction("touch of weakness", ACTION_HIGH + 6),
			new NextAction("devouring plague", ACTION_HIGH + 6),
			new NextAction("hex of weakness", ACTION_HIGH + 6),
			new NextAction("war stomp", ACTION_HIGH + 6),
			NULL)));

	triggers.push_back(new TriggerNode(
		"often",
		NextAction::array(0, new NextAction("fear ward", ACTION_EMERGENCY + 6), NULL)));
	
	triggers.push_back(new TriggerNode(
		"fear ward",
		NextAction::array(0, new NextAction("fear ward", ACTION_NORMAL + 6), NULL)));
	
	triggers.push_back(new TriggerNode(
		"fear ward on party",
		NextAction::array(0, new NextAction("fear ward on party", ACTION_NORMAL + 6), NULL)));

	triggers.push_back(new TriggerNode(
		"panic",
		NextAction::array(0, new NextAction("shadowmeld", ACTION_EMERGENCY + 6), NULL)));

	triggers.push_back(new TriggerNode(
		"enemy player is attacking",
		NextAction::array(0, new NextAction("shadowguard", ACTION_EMERGENCY + 6), NULL)));
}

RacialsStrategy::RacialsStrategy(PlayerbotAI* ai) : Strategy(ai)
{
    actionNodeFactories.Add(new RacialsStrategyActionNodeFactory());
}
