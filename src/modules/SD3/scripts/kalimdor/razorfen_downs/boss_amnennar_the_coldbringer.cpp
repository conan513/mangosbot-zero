/*
Scripted by Lee Prenerf is included.
*/

#include "precompiled.h"
#include "razorfen_downs.h"

enum
{
    SAY_AGGRO               = -1129000,
    SAY_SUMMON60            = -1129001,
    SAY_SUMMON30            = -1129002,
    SAY_HP                  = -1129003,
    SAY_KILL                = -1129004,

    SPELL_AMNENNARSWRATH    = 13009,
    SPELL_FROSTBOLT         = 15530,
    SPELL_FROST_NOVA        = 15531,
    SPELL_FROST_SPECTRES    = 12642
};

struct boss_coldbringer : public CreatureScript
{
    boss_coldbringer() : CreatureScript("boss_coldbringer") {}

    struct boss_coldbringerAI : public ScriptedAI
    {
        boss_coldbringerAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        uint32 AmnenarsWrath_Timer;
        uint32 FrostBolt_Timer;
        uint32 FrostNova_Timer;
        bool Spectrals70;
        bool Spectrals55;
        bool Spectrals30;
        bool Hp;

        void Reset() override
        {
            AmnenarsWrath_Timer = 8000;
            FrostBolt_Timer = 1000;
            FrostNova_Timer = 10000 + rand() % 5000;
            Spectrals70 = false;
            Spectrals55 = false;
            Spectrals30 = false;
            Hp = false;
        }

        void Aggro(Unit* /*pWho*/) override
        {
            DoScriptText(SAY_AGGRO, m_creature);
        }

        void KilledUnit(Unit* pVictim) override
        {
            DoScriptText(SAY_KILL, m_creature);
        }


        void UpdateAI(const uint32 diff) override
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            {
                return;
            }

            //AmnenarsWrath_Timer
            if (AmnenarsWrath_Timer <= diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_AMNENNARSWRATH);
                AmnenarsWrath_Timer = 12000;
            }
            else AmnenarsWrath_Timer -= diff;

            //FrostBolt_Timer
            if (FrostBolt_Timer <= diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT);
                FrostBolt_Timer = 8000;
            }
            else FrostBolt_Timer -= diff;

            //FrostNova_Timer
            if (FrostNova_Timer <= diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_FROST_NOVA);
                FrostNova_Timer = 15000;
            }
            else FrostNova_Timer -= diff;

            //Summon First group of adds at 70%
            if (!Spectrals70 && m_creature->HealthBelowPct(70))
            {
                DoScriptText(SAY_SUMMON60, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_FROST_SPECTRES);
                Spectrals70 = true;
            }

            //if health at 50% Say something
            if (!Hp && m_creature->HealthBelowPct(50))
            {
                DoScriptText(SAY_HP, m_creature);
                Hp = true;
            }
            //Summon Second group of adds at 55%
            if (!Spectrals55 && m_creature->HealthBelowPct(55))
            {
                DoScriptText(SAY_SUMMON30, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_FROST_SPECTRES);
                Spectrals55 = true;
            }

            //Summon 3 group of creatures this is before prenerf!
            if (!Spectrals30 && m_creature->HealthBelowPct(30))
            {
                DoScriptText(SAY_SUMMON30, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_FROST_SPECTRES);
                Spectrals30 = true;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) override
    {
        return new boss_coldbringerAI(pCreature);
    }
};

void AddSC_boss_coldbringerAI()
{
    Script* s;
    s = new boss_coldbringer();
    s->RegisterSelf();
}