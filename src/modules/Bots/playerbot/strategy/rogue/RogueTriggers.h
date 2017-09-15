#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{

    class KickInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        KickInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "kick") {}
    };

	class BlindInterruptSpellTrigger : public InterruptSpellTrigger
	{
	public:
		BlindInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "blind") {}
	};

    class SliceAndDiceTrigger : public BuffTrigger
    {
    public:
        SliceAndDiceTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "slice and dice") {}
    };

    class RuptureTrigger : public DebuffTrigger
    {
    public:
        RuptureTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "rupture") {}
    };

    class ExposeArmorTrigger : public DebuffTrigger
    {
    public:
        ExposeArmorTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "expose armor") {}
    };

    class KickInterruptEnemyHealerSpellTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        KickInterruptEnemyHealerSpellTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "kick") {}
    };

	class BlindInterruptEnemyHealerSpellTrigger : public InterruptEnemyHealerTrigger
	{
	public:
		BlindInterruptEnemyHealerSpellTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "blind") {}
	};

	class StealthTrigger : public BuffTrigger
	{
	public:
		StealthTrigger(PlayerbotAI* ai) : BuffTrigger(ai, "stealth") {}
		virtual bool IsActive() { return !ai->HasAura("stealth", bot); }
	};

	class RogueCanCheapShotTrigger : public Trigger {
	public:
		RogueCanCheapShotTrigger(PlayerbotAI* ai) : Trigger(ai, "cheap shot open") {}
		virtual bool IsActive() 
		{
			return ai->HasAura("stealth", bot);
			{
			Unit* target = AI_VALUE(Unit*, "current target");
			return target && AI_VALUE2(float, "distance", "current target") <= sPlayerbotAIConfig.meleeDistance;
			}
		}
	};

	class RogueCanOpenFromBehindTrigger : public Trigger {
	public:
		RogueCanOpenFromBehindTrigger(PlayerbotAI* ai) : Trigger(ai, "can open from behind") {}
		virtual bool IsActive()
		{
			return ai->HasAura("stealth", bot);
			{
				Unit* target = AI_VALUE(Unit*, "current target");
				return target && AI_VALUE2(bool, "behind", "current target");
				{
					Unit* target = AI_VALUE(Unit*, "current target");
					return target && AI_VALUE2(float, "distance", "current target") <= sPlayerbotAIConfig.meleeDistance;
				}
			}
		}
	};

	class SinisterStrikeTrigger : public SpellCanBeCastTrigger
	{
	public:
		SinisterStrikeTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "sinister strike") {}
	};

	class MutilateTrigger : public SpellCanBeCastTrigger
	{
	public:
		MutilateTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "mutilate") {}
	};

	class HemorrhageTrigger : public SpellCanBeCastTrigger
	{
	public:
		HemorrhageTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "hemorrhage") {}
	};
}
