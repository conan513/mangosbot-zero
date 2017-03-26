#pragma once

#include "../game/Tools/Language.h"
#include "../shared/Log/Log.h"
#include "../game/WorldHandlers/World.h"
#include "../game/Object/ObjectMgr.h"
#include "../game/Server/WorldSession.h"
#include "../shared/Config/Config.h"
#include "../shared/Utilities/Util.h"
#include "../game/WorldHandlers/AccountMgr.h"
#include "../game/WorldHandlers/MapManager.h"
#include "../game/Object/Player.h"
#include "../game/WorldHandlers/Chat.h"
#include "../shared/Database/DatabaseEnv.h"
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestPath.h>
#include <stdexcept>

#include <stdio.h>
#include <iostream>

#include <list>
#include "../framework/Platform/Define.h"

#include <stdexcept>

#include "../../modules/Bots/playerbot/playerbotDefs.h"
