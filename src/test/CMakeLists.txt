# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_cppunit cppunit/*.cpp cppunit/*.h)
source_group("CppUnit" FILES ${sources_cppunit})
file(GLOB_RECURSE sources_efficiency efficiency/*.cpp efficiency/*.h)
source_group("Efficency" FILES ${sources_efficiency})
file(GLOB_RECURSE sources_main *.cpp *.h)
source_group("Main" FILES ${sources_main})

# Create game-libary

if (PCH)
  set(tests_STAT_PCH_HDR pch.h)
  set(tests_STAT_PCH_SRC pch.cpp)
endif ()

set(tests_STAT_SRCS
  ${tests_STAT_SRCS}
  ${sources_cppunit}
  ${sources_efficiency}
  ${sources_main}
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/shared
    ${CMAKE_SOURCE_DIR}/src/shared/Common
    ${CMAKE_SOURCE_DIR}/src/shared/Config
    ${CMAKE_SOURCE_DIR}/src/shared/Threading
    ${CMAKE_SOURCE_DIR}/src/shared/Utilities
    ${CMAKE_SOURCE_DIR}/src/shared/Log
    ${CMAKE_SOURCE_DIR}/src/shared/DataStores
    ${CMAKE_SOURCE_DIR}/src/shared/Win
    ${CMAKE_SOURCE_DIR}/src/shared/Linux
    ${CMAKE_SOURCE_DIR}/src/framework
    ${CMAKE_SOURCE_DIR}/src/game/AuctionHouseBot
    ${CMAKE_SOURCE_DIR}/src/game/BattleGround
    ${CMAKE_SOURCE_DIR}/src/game/Server
    ${CMAKE_SOURCE_DIR}/src/game/WorldHandlers
    ${CMAKE_SOURCE_DIR}/src/game/Maps
    ${CMAKE_SOURCE_DIR}/src/game/Object
    ${CMAKE_SOURCE_DIR}/src/game/Tools
    ${CMAKE_SOURCE_DIR}/src/game/References
    ${CMAKE_SOURCE_DIR}/src/game/MotionGenerators
    ${CMAKE_SOURCE_DIR}/src/game/vmap
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/shared
    ${ACE_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
    ${MYSQL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src/test
    ${CMAKE_SOURCE_DIR}/src/test/cppunit
    ${CMAKE_SOURCE_DIR}/src/modules/Bots/playerbot
    ${CMAKE_SOURCE_DIR}/src/modules/Bots/ahbot
)

add_executable(tests
  ${tests_STAT_SRCS}
  ${tests_STAT_PCH_SRC}
)

if(NOT ACE_USE_EXTERNAL)
    add_dependencies(tests ace)
    target_link_libraries(tests ace)
else()
    target_link_libraries(tests ACE)
endif()

target_link_libraries(tests
  game
  shared
  framework
  g3dlite
  ${OPENSSL_LIBRARIES}
  ${MYSQL_LIBRARY}
)

add_dependencies(tests game)

# Generate precompiled header
if (PCH)
  add_cxx_pch(tests ${tests_STAT_PCH_HDR} ${tests_STAT_PCH_SRC})
endif ()
