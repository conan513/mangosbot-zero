#include "botpch.h"
#include "../../playerbot.h"
#include "MageMultipliers.h"
#include "ArcaneMageStrategy.h"

using namespace ai;

class ArcaneMageStrategyActionNodeFactory : public NamedObjectFactory<ActionNode>
{
public:
    ArcaneMageStrategyActionNodeFactory()
    {
        creators["arcane missiles"] = &arcane_missiles;
    }
private:
    static ActionNode* arcane_missiles(PlayerbotAI* ai)
    {
        return new ActionNode ("arcane missiles",
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("shoot"), NULL),
            /*C*/ NULL);
    }
};

ArcaneMageStrategy::ArcaneMageStrategy(PlayerbotAI* ai) : GenericMageStrategy(ai)
{
    actionNodeFactories.Add(new ArcaneMageStrategyActionNodeFactory());
}

NextAction** ArcaneMageStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("arcane missiles", 10.0f), NULL);
}

void ArcaneMageStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericMageStrategy::InitTriggers(triggers);


}

