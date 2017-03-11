#include "pch.h"
#include "aitest.h"

#include "EngineTestBase.h"
#include "MockedAiObjectContextTestCase.h"

using namespace ai;


class NonCombatEngineTestCase2 : public MockedAiObjectContextTestCase
{
  CPPUNIT_TEST_SUITE( NonCombatEngineTestCase2 );
      CPPUNIT_TEST( emote );
      CPPUNIT_TEST( suggest );
      CPPUNIT_TEST( ready_check );
      CPPUNIT_TEST( reveal );
      CPPUNIT_TEST( gather );
      CPPUNIT_TEST( greet );
  CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		EngineTestBase::setUp();
        setupEngine(context = new MockAiObjectContext(ai, new AiObjectContext(ai), &ai->buffer), "pvp", NULL);
	}

protected:
    void emote()
    {
        engine->addStrategy("emote");
        tickWithTrigger("random");
        tickWithTrigger("random");

        assertActions(">S:emote>S:suggest trade");
    }

    void suggest()
    {
        engine->addStrategy("emote");
        tickWithTrigger("seldom");

        assertActions(">S:suggest what to do");
    }

    void ready_check()
    {
        engine->addStrategy("ready check");
        tickWithTrigger("timer");

        assertActions(">S:ready check");
    }

    void reveal()
    {
        engine->addStrategy("reveal");
        tickWithTrigger("often");

        assertActions(">S:reveal gathering item");
    }


    void gather()
    {
        engine->addStrategy("stay");
        engine->addStrategy("gather");
        engine->addStrategy("loot");

        tickWithTrigger("timer");

        assertActions(">S:add gathering loot");
    }

    void greet()
    {
        engine->addStrategy("emote");
        tickWithTrigger("new player nearby");

        assertActions(">S:greet");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( NonCombatEngineTestCase2 );
