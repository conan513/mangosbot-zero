#include "botpch.h"
#include "../../playerbot.h"
#include "DpsTargetValue.h"

using namespace ai;

class FindTargetForDpsStrategy : public FindTargetStrategy
{
public:
	FindTargetForDpsStrategy(PlayerbotAI* ai) : FindTargetStrategy(ai)
    {
		minThreat = 0;
		maxTankCount = 0;
		minDpsCount = 0;
    }

public:
	virtual void CheckAttacker(Unit* attacker, ThreatManager* threatManager)
    {
        Group* group = ai->GetBot()->GetGroup();
        float threat = threatManager->getThreat(ai->GetBot());
        int tankCount, dpsCount;
        GetPlayerCount(attacker, &tankCount, &dpsCount);
		if (group)
        {
            uint64 guid = group->GetTargetIcon(4);
            if (guid && attacker->GetObjectGuid() == ObjectGuid(guid))
                return;
        }
		
		if (!result ||
			minThreat >= threat && (maxTankCount <= tankCount || minDpsCount >= dpsCount))
		{
			minThreat = threat;
			maxTankCount = tankCount;
			minDpsCount = dpsCount;
			result = attacker;
		}
    }

protected:
	float minThreat;
	int maxTankCount;
	int minDpsCount;
};

Unit* DpsTargetValue::Calculate()
{
	FindTargetForDpsStrategy strategy(ai);
	return FindTarget(&strategy);
}
