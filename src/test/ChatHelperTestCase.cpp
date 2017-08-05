#include "pch.h"

#include "aitest.h"
#include "MockPlayerbotAIBase.h"
#include "Helpers.h"

using namespace ai;

class ChatHelperTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( ChatHelperTestCase );
      CPPUNIT_TEST( filter );
  CPPUNIT_TEST_SUITE_END();

protected:
    vector<int> src;

public:
	void setUp()
	{
	    src.push_back(1);
	    src.push_back(2);
	    src.push_back(3);
	    src.push_back(4);
	    src.push_back(5);
	}

protected:
    void filter()
	{
        assertFilter("", "1,2,3,4,5");
        assertFilter("*", "1,2,3,4,5");
        assertFilter("abc", "1");
        assertFilter("1,3", "1,3");
        assertFilter("0,17", "1,5");
        assertFilter("2-4", "2,3,4");
        assertFilter("-3", "1,2,3");
        assertFilter("2-", "2,3,4,5");
    }

    void assertFilter(string filter, string expected)
    {
        map<int, int> result = filterList(src, filter);
        ostringstream out;
        for (map<int, int>::iterator i = result.begin(); i != result.end(); ++i)
        {
            if (!out.str().empty()) out << ",";
            out << i->second;
        }
        cout << filter << " = " << out.str() << "\n";
        CPPUNIT_ASSERT(out.str() == expected);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( ChatHelperTestCase );
