cmake_minimum_required(VERSION 2.8)

PROJECT(robot_config)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

file(COPY "../../battle_server/protocol/robot_generated.h" DESTINATION "./tools/")

file(COPY "../../battle_server/tools/Packet_.h" DESTINATION "./tools/")
file(COPY "../../battle_server/tools/Packet_.cpp" DESTINATION "./tools/")
file(COPY "../../battle_server/tools/Handle.h" DESTINATION "./tools/")

file(COPY "../../battle_server/src/PackMsg.h" DESTINATION "./tools/")
file(COPY "../../battle_server/src/PackMsg.cpp" DESTINATION "./tools/")

file(COPY "../../battle_server/src/Redirect.cpp" DESTINATION "./tools/")


file(COPY "../../battle_server/src/StatCount.h" DESTINATION "./tools/")
file(COPY "../../battle_server/src/StatCount.cpp" DESTINATION "./tools/")

file(COPY "../../battle_server/src/ManageStat.h" DESTINATION "./tools/")
file(COPY "../../battle_server/src/ManageStat.cpp" DESTINATION "./tools/")

include_directories(. ./tools ./protocol ../../deps ../../battle_server/inc ../../battle_server/tools ../../battle_server/src ../netcore/inc ../simplexml/inc ../logger/inc $ENV{BOOST_INC} $ENV{EVENT_INC})
LINK_DIRECTORIES(/home/server_trunk/deps/libs $ENV{BOOST_LIB} $ENV{LEV_LIB} /open_source/RakNet-master/Lib/DLL)
aux_source_directory(. SRC_FILE)
aux_source_directory(./tools TOOLS_FILE)
aux_source_directory(./protocol TOOLS_FILE)
ADD_EXECUTABLE(robot_config ${SRC_FILE} ${TOOLS_FILE})
TARGET_LINK_LIBRARIES(robot_config simplexml)
TARGET_LINK_LIBRARIES(robot_config logger)
TARGET_LINK_LIBRARIES(robot_config event_core)
TARGET_LINK_LIBRARIES(robot_config netcore)
TARGET_LINK_LIBRARIES(robot_config boost_thread)
TARGET_LINK_LIBRARIES(robot_config boost_date_time)
TARGET_LINK_LIBRARIES(robot_config boost_filesystem)
TARGET_LINK_LIBRARIES(robot_config RakNetDLL)


