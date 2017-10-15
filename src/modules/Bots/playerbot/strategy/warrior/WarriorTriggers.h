#pragma once
#include "../triggers/GenericTriggers.h"

namespace ai
{
    BUFF_TRIGGER(BattleShoutTrigger, "battle shout", "battle shout")

    DEBUFF_TRIGGER(RendDebuffTrigger, "rend", "rend")
    DEBUFF_TRIGGER(DisarmDebuffTrigger, "disarm", "disarm")
    DEBUFF_TRIGGER(SunderArmorDebuffTrigger, "sunder armor", "sunder armor")

    class RendDebuffOnAttackerTrigger : public DebuffOnAttackerTrigger
    {
    public:
        RendDebuffOnAttackerTrigger(PlayerbotAI* ai) : DebuffOnAttackerTrigger(ai, "rend") {}
    };

	class RevengeAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		RevengeAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "revenge") {}
	};

	class ShieldSlamAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		ShieldSlamAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "shield slam") {}
	};

	class ShieldBlockAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		ShieldBlockAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "shield block") {}
	};

	class ShieldBashAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		ShieldBashAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "shield bash") {}
	};

	class BloodthirstAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		BloodthirstAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "bloodthirst") {}
	};

	class MortalStrikeAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		MortalStrikeAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "mortal strike") {}
	};

	class WhirlwindAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		WhirlwindAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "whirlwind") {}
	};

	class OverpowerAvailableTrigger : public SpellCanBeCastTrigger
	{
	public:
		OverpowerAvailableTrigger(PlayerbotAI* ai) : SpellCanBeCastTrigger(ai, "overpower") {}
	};

    class BloodrageDebuffTrigger : public DebuffTrigger
    {
    public:
        BloodrageDebuffTrigger(PlayerbotAI* ai) : DebuffTrigger(ai, "bloodrage") {}
        virtual bool IsActive()
        {
            return DebuffTrigger::IsActive() &&
                AI_VALUE2(uint8, "health", "self target") >= 75 &&
                AI_VALUE2(uint8, "rage", "self target") < 20;
        }
    };

    class ShieldBashInterruptSpellTrigger : public InterruptSpellTrigger
    {
    public:
        ShieldBashInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "shield bash") {}
    };
	
	class PummelInterruptSpellTrigger : public InterruptSpellTrigger
	{
	public:
		PummelInterruptSpellTrigger(PlayerbotAI* ai) : InterruptSpellTrigger(ai, "pummel") {}
	};

    class ConcussionBlowTrigger : public SnareTargetTrigger
    {
    public:
        ConcussionBlowTrigger(PlayerbotAI* ai) : SnareTargetTrigger(ai, "concussion blow") {}
    };

    class HamstringTrigger : public SnareTargetTrigger
    {
    public:
        HamstringTrigger(PlayerbotAI* ai) : SnareTargetTrigger(ai, "hamstring") {}
    };

    class DeathWishTrigger : public BoostTrigger
    {
    public:
        DeathWishTrigger(PlayerbotAI* ai) : BoostTrigger(ai, "death wish") {}
    };

	class SweepingStrikesTrigger : public BoostTrigger
    {
    public:
		SweepingStrikesTrigger(PlayerbotAI* ai) : BoostTrigger(ai, "sweeping strikes") {}
    };

    class ShieldBashInterruptEnemyHealerSpellTrigger : public InterruptEnemyHealerTrigger
    {
    public:
        ShieldBashInterruptEnemyHealerSpellTrigger(PlayerbotAI* ai) : InterruptEnemyHealerTrigger(ai, "shield bash") {}
    };

}
