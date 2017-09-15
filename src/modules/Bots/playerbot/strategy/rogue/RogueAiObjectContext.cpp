#include "botpch.h"
#include "../../playerbot.h"
#include "RogueActions.h"
#include "RogueTriggers.h"
#include "RogueAiObjectContext.h"
#include "DpsRogueStrategy.h"
#include "CombatDpsRogueStrategy.h"
#include "SubDpsRogueStrategy.h"
#include "GenericRogueNonCombatStrategy.h"
#include "../generic/PullStrategy.h"
#include "../NamedObjectContext.h"

using namespace ai;


namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class StrategyFactoryInternal : public NamedObjectContext<Strategy>
        {
        public:
            StrategyFactoryInternal()
            {
				creators["assa dps"] = &rogue::StrategyFactoryInternal::assa_dps;
				creators["combat dps"] = &rogue::StrategyFactoryInternal::combat_dps;
                creators["sub dps"] = &rogue::StrategyFactoryInternal::sub_dps;
                creators["nc"] = &rogue::StrategyFactoryInternal::nc;
                creators["pull"] = &rogue::StrategyFactoryInternal::pull;
            }

        private:
            static Strategy* assa_dps(PlayerbotAI* ai) { return new DpsRogueStrategy(ai); }
			static Strategy* combat_dps(PlayerbotAI* ai) { return new CombatDpsRogueStrategy(ai); }
			static Strategy* sub_dps(PlayerbotAI* ai) { return new SubDpsRogueStrategy(ai); }
            static Strategy* nc(PlayerbotAI* ai) { return new GenericRogueNonCombatStrategy(ai); }
            static Strategy* pull(PlayerbotAI* ai) { return new PullStrategy(ai, "shoot"); }
        };
    };
};

namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class TriggerFactoryInternal : public NamedObjectContext<Trigger>
        {
        public:
            TriggerFactoryInternal()
            {
                creators["kick"] = &TriggerFactoryInternal::kick;
				creators["blind"] = &TriggerFactoryInternal::blind;
                creators["rupture"] = &TriggerFactoryInternal::rupture;
                creators["slice and dice"] = &TriggerFactoryInternal::slice_and_dice;
                creators["expose armor"] = &TriggerFactoryInternal::expose_armor;
                creators["kick on enemy healer"] = &TriggerFactoryInternal::kick_on_enemy_healer;
				creators["blind on enemy healer"] = &TriggerFactoryInternal::blind_on_enemy_healer;
				creators["cheap shot open"] = &TriggerFactoryInternal::cheap_shot_open;
				creators["can open from behind"] = &TriggerFactoryInternal::can_open_from_behind;
				creators["sinister strike"] = &TriggerFactoryInternal::sinister_strike;
				creators["mutilate"] = &TriggerFactoryInternal::mutilate;
				creators["hemorrhage"] = &TriggerFactoryInternal::hemorrhage;
            }

        private:
            static Trigger* kick(PlayerbotAI* ai) { return new KickInterruptSpellTrigger(ai); }
			static Trigger* blind(PlayerbotAI* ai) { return new BlindInterruptSpellTrigger(ai); }
            static Trigger* rupture(PlayerbotAI* ai) { return new RuptureTrigger(ai); }
            static Trigger* slice_and_dice(PlayerbotAI* ai) { return new SliceAndDiceTrigger(ai); }
            static Trigger* expose_armor(PlayerbotAI* ai) { return new ExposeArmorTrigger(ai); }
            static Trigger* kick_on_enemy_healer(PlayerbotAI* ai) { return new KickInterruptEnemyHealerSpellTrigger(ai); }
			static Trigger* blind_on_enemy_healer(PlayerbotAI* ai) { return new BlindInterruptEnemyHealerSpellTrigger(ai); }
			static Trigger* cheap_shot_open(PlayerbotAI* ai) { return new RogueCanCheapShotTrigger(ai); }
			static Trigger* can_open_from_behind(PlayerbotAI* ai) { return new RogueCanOpenFromBehindTrigger(ai); }
			static Trigger* sinister_strike(PlayerbotAI* ai) { return new SinisterStrikeTrigger(ai); }
			static Trigger* mutilate(PlayerbotAI* ai) { return new MutilateTrigger(ai); }
			static Trigger* hemorrhage(PlayerbotAI* ai) { return new HemorrhageTrigger(ai); }
        };
    };
};


namespace ai
{
    namespace rogue
    {
        using namespace ai;

        class AiObjectContextInternal : public NamedObjectContext<Action>
        {
        public:
            AiObjectContextInternal()
            {
                creators["riposte"] = &AiObjectContextInternal::riposte;
                creators["mutilate"] = &AiObjectContextInternal::mutilate;
				creators["hemorrhage"] = &AiObjectContextInternal::hemorrhage;
                creators["sinister strike"] = &AiObjectContextInternal::sinister_strike;
                creators["kidney shot"] = &AiObjectContextInternal::kidney_shot;
                creators["rupture"] = &AiObjectContextInternal::rupture;
                creators["slice and dice"] = &AiObjectContextInternal::slice_and_dice;
                creators["eviscerate"] = &AiObjectContextInternal::eviscerate;
                creators["vanish"] = &AiObjectContextInternal::vanish;
                creators["evasion"] = &AiObjectContextInternal::evasion;
                creators["kick"] = &AiObjectContextInternal::kick;
				creators["blind"] = &AiObjectContextInternal::blind;
                creators["feint"] = &AiObjectContextInternal::feint;
                creators["backstab"] = &AiObjectContextInternal::backstab;
                creators["expose armor"] = &AiObjectContextInternal::expose_armor;
                creators["kick on enemy healer"] = &AiObjectContextInternal::kick_on_enemy_healer;
				creators["stealth"] = &AiObjectContextInternal::stealth;
				creators["cheap shot"] = &AiObjectContextInternal::cheap_shot;
				creators["garrote"] = &AiObjectContextInternal::garrote;
            }

        private:
            static Action* riposte(PlayerbotAI* ai) { return new CastRiposteAction(ai); }
            static Action* mutilate(PlayerbotAI* ai) { return new CastMutilateAction(ai); }
            static Action* hemorrhage(PlayerbotAI* ai) { return new CastHemorrhageAction(ai); }
			static Action* sinister_strike(PlayerbotAI* ai) { return new CastSinisterStrikeAction(ai); }
            static Action* kidney_shot(PlayerbotAI* ai) { return new CastKidneyShotAction(ai); }
            static Action* rupture(PlayerbotAI* ai) { return new CastRuptureAction(ai); }
            static Action* slice_and_dice(PlayerbotAI* ai) { return new CastSliceAndDiceAction(ai); }
            static Action* eviscerate(PlayerbotAI* ai) { return new CastEviscerateAction(ai); }
            static Action* vanish(PlayerbotAI* ai) { return new CastVanishAction(ai); }
            static Action* evasion(PlayerbotAI* ai) { return new CastEvasionAction(ai); }
            static Action* kick(PlayerbotAI* ai) { return new CastKickAction(ai); }
			static Action* blind(PlayerbotAI* ai) { return new CastBlindAction(ai); }
            static Action* feint(PlayerbotAI* ai) { return new CastFeintAction(ai); }
            static Action* backstab(PlayerbotAI* ai) { return new CastBackstabAction(ai); }
            static Action* expose_armor(PlayerbotAI* ai) { return new CastExposeArmorAction(ai); }
            static Action* kick_on_enemy_healer(PlayerbotAI* ai) { return new CastKickOnEnemyHealerAction(ai); }
			static Action* blind_on_enemy_healer(PlayerbotAI* ai) { return new CastBlindOnEnemyHealerAction(ai); }
			static Action* stealth(PlayerbotAI* ai) { return new CastStealthAction(ai); }
			static Action* cheap_shot(PlayerbotAI* ai) { return new CastCheapShotAction(ai); }
			static Action* garrote(PlayerbotAI* ai) { return new CastGarroteAction(ai); 
			}
        };
    };
};

RogueAiObjectContext::RogueAiObjectContext(PlayerbotAI* ai) : AiObjectContext(ai)
{
    strategyContexts.Add(new ai::rogue::StrategyFactoryInternal());
    actionContexts.Add(new ai::rogue::AiObjectContextInternal());
    triggerContexts.Add(new ai::rogue::TriggerFactoryInternal());
}
