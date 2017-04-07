#include "pch.h"

#include "aitest.h"
#include "../../modules/Bots/playerbot/strategy/shaman/ShamanAiObjectContext.h"

using namespace ai;


class HealShamanTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( HealShamanTestCase );
    CPPUNIT_TEST( healHimself );
    CPPUNIT_TEST( healOthers );
    CPPUNIT_TEST( aoe_heal );
    CPPUNIT_TEST( buff );
    CPPUNIT_TEST( interruptSpell );
	CPPUNIT_TEST( dispel );
	CPPUNIT_TEST( cureCurse );
	CPPUNIT_TEST( cureDisease );
	CPPUNIT_TEST( curePoison );
	CPPUNIT_TEST( lowMana );
	CPPUNIT_TEST( range );
	CPPUNIT_TEST( aoe );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new ShamanAiObjectContext(ai), "heal", NULL);

        addAura("water shield");
    }

protected:
    void healHimself()
    {
        tickWithLowHealth(45);
		tickWithLowHealth(35);
        tickWithLowHealth(35);

        assertActions(">S:lesser healing wave>S:riptide>S:healing wave");
    }

    void healOthers()
    {
        tickWithPartyLowHealth(45);
        tickWithPartyLowHealth(35);
		tickWithPartyLowHealth(35);

        assertActions(">P:lesser healing wave on party>P:riptide on party>P:healing wave on party");
    }

    void aoe_heal()
    {
        tickWithAoeHeal("medium");
        tickWithAoeHeal("medium");

        assertActions(">P:chain heal>P:lesser healing wave on party");
    }

    void buff()
    {
        engine->addStrategy("bmana");
        removeAura("water shield");

        tick();
        addAura("water shield");

        tickWithItemForSpell("earthliving weapon");
        tickWithItemForSpell("earthliving weapon");
        tickWithItemForSpell("earthliving weapon");
        tickWithItemForSpell("earthliving weapon");

        engine->addStrategy("totems");
        tick();
        tick();
        tick();
        tick();

        assertActions(">S:water shield>S:earthliving weapon>S:flametongue weapon>S:frostbrand weapon>S:rockbiter weapon>S:mana spring totem>S:strength of earth totem>S:flametongue totem>S:grace of air totem");
    }

    void interruptSpell()
    {
        tickWithTargetIsCastingNonMeleeSpell();

		assertActions(">T:wind shear");
    }

	void dispel()
	{
		tickWithTargetAuraToDispel(DISPEL_MAGIC);

		assertActions(">T:purge");
	}

	void cureCurse()
	{
	    engine->addStrategy("cure");
		tickWithAuraToDispel(DISPEL_CURSE);

		spellAvailable("cleanse spirit");
        tickWithPartyAuraToDispel(DISPEL_CURSE);

		assertActions(">S:cleanse spirit>P:cleanse spirit curse on party");
	}

	void cureDisease()
	{
        engine->addStrategy("cure");
		tickWithAuraToDispel(DISPEL_DISEASE);
        tickWithAuraToDispel(DISPEL_DISEASE);
        spellUnavailable("cleanse spirit");

        spellAvailable("cleanse spirit");
        spellAvailable("cure disease");
        tickWithPartyAuraToDispel(DISPEL_DISEASE);
        tickWithPartyAuraToDispel(DISPEL_DISEASE);

		assertActions(">S:cure disease>S:cleanse spirit>P:cure disease on party>P:cleanse spirit disease on party");
	}

    void curePoison()
    {
        engine->addStrategy("cure");
        tickWithAuraToDispel(DISPEL_POISON);
        spellUnavailable("cleanse spirit");
        tickWithAuraToDispel(DISPEL_POISON);

        spellAvailable("cleanse spirit");
        spellAvailable("cure poison");
        tickWithPartyAuraToDispel(DISPEL_POISON);
        spellUnavailable("cleanse spirit");
        tickWithPartyAuraToDispel(DISPEL_POISON);

        assertActions(">S:cleanse spirit>S:cure poison>P:cleanse spirit poison on party>P:cure poison on party");
    }

	void lowMana()
	{
	    tickWithLowMana(1);
		assertActions(">S:mana tide totem");
	}

    void range()
    {
        tickOutOfSpellRange();

        assertActions(">T:reach spell");
    }

    void aoe()
    {
        tickWithAoeCount(3);

        assertActions(">S:healing stream totem");
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( HealShamanTestCase );
