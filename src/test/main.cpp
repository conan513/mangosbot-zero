#include "pch.h"
#include "../../modules/Bots/playerbot/PlayerbotAIConfig.h"
#include "../../modules/Bots/ahbot/AhBot.h"

DatabaseType WorldDatabase;                                 ///< Accessor to the world database
DatabaseType CharacterDatabase;                             ///< Accessor to the character database
DatabaseType LoginDatabase;                                 ///< Accessor to the realm/login database
uint32 realmID;                                             ///< Id of the realm

int main(int argc, char* argv[])
{

    /*std::string configError;
    if (!sConfigMgr->LoadInitial("worldserver.conf", configError))
    {
        printf("Error in config file: %s\n", configError.c_str());
        return 1;
    }

    MySQL::Library_Init();

    WorldDatabase.SetConnectionInfo(sConfigMgr->GetStringDefault("WorldDatabaseInfo", ""), 1, 1);
    if (!WorldDatabase.Open())
    {
        printf("Cannot connect to world database");
        return false;
    }
    CharacterDatabase.SetConnectionInfo(sConfigMgr->GetStringDefault("CharacterDatabaseInfo", ""), 1, 1);
    if (!CharacterDatabase.Open())
    {
        printf("Cannot connect to Character database");
        return false;
    }
    LoginDatabase.SetConnectionInfo(sConfigMgr->GetStringDefault("LoginDatabaseInfo", ""), 1, 1);
    if (!LoginDatabase.Open())
    {
        printf("Cannot connect to Login database");
        return false;
    }

    //sWorld->SetInitialWorldSettings();
    std::string dataPath = sConfigMgr->GetStringDefault("DataDir", "./");
    if (dataPath.empty() || (dataPath.at(dataPath.length()-1) != '/' && dataPath.at(dataPath.length()-1) != '\\'))
        dataPath.push_back('/');

    ///- Load the DBC files
     TC_LOG_INFO("server.loading", "Initialize data stores...");
     LoadDBCStores(dataPath);

     auctionbot.Init();
*/

    //sPlayerbotAIConfig.Initialize();
    sPlayerbotAIConfig.enabled = true;
    sPlayerbotAIConfig.globalCoolDown = 1000;
    sPlayerbotAIConfig.reactDelay = 100;
    sPlayerbotAIConfig.sightDistance = 40.0f;
    sPlayerbotAIConfig.spellDistance = 25.0f;
    sPlayerbotAIConfig.shootDistance = 10.0f;
    sPlayerbotAIConfig.reactDistance = 100.0f;
    sPlayerbotAIConfig.grindDistance = 50.0f;
    sPlayerbotAIConfig.meleeDistance = ATTACK_DISTANCE;
    sPlayerbotAIConfig.followDistance = ATTACK_DISTANCE;
    sPlayerbotAIConfig.tooCloseDistance = ATTACK_DISTANCE;
    sPlayerbotAIConfig.contactDistance = CONTACT_DISTANCE;
    sPlayerbotAIConfig.lootDistance = 20.0f;
    sPlayerbotAIConfig.criticalHealth = 25;
    sPlayerbotAIConfig.lowHealth = 40;
    sPlayerbotAIConfig.mediumHealth = 60;
    sPlayerbotAIConfig.almostFullHealth = 80;
    sPlayerbotAIConfig.lowMana = 40;
    sPlayerbotAIConfig.mediumMana = 60;
    sPlayerbotAIConfig.iterationsPerTick = 10;
    sPlayerbotAIConfig.randomChangeMultiplier = 1;

    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that colllects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener( &result );

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener( &progress );

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;

    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run( controller, argc > 1 ? argv[1] : "" );

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    outputter.write();

    cout.flush();
    return result.wasSuccessful() ? 0 : 1;
}



bool ChatHandler::HandleAccountDeleteCommand(char* args)
{
    return true;
}

void ChatHandler::HandleCharacterDeletedListHelper(DeletedInfoList const& foundList)
{
}

bool ChatHandler::HandleCharacterDeletedListCommand(char* args)
{
    return true;
}

void ChatHandler::HandleCharacterDeletedRestoreHelper(DeletedInfo const& delInfo)
{
}

bool ChatHandler::HandleCharacterDeletedRestoreCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleCharacterDeletedDeleteCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleCharacterDeletedOldCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleCharacterEraseCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleQuitCommand(char* /*args*/)
{
    return true;
}

bool ChatHandler::HandleServerExitCommand(char* /*args*/)
{
    return true;
}

bool ChatHandler::HandleAccountOnlineListCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleAccountCreateCommand(char* args)
{
    return true;
}

bool ChatHandler::HandleServerLogFilterCommand(char* args)
{
    return false;
}

bool ChatHandler::HandleServerLogLevelCommand(char* args)
{
    return true;
}


