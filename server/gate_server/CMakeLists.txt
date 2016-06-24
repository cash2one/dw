cmake_minimum_required(VERSION 2.8)

PROJECT(gatesys)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

file(COPY "../battle_server/protocol/guid_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/protocol_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/check_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/common_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/framecmd_c2c_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/framecmd_c2s_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/framesync_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/guid_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/inter_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/lobby_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/login_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/protocol_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/robot_generated.h" DESTINATION "./tools/")

file(COPY "../battle_server/tools/Packet_.h" DESTINATION "./tools/")
file(COPY "../battle_server/tools/Packet_.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/Handle.h" DESTINATION "./tools/")

file(COPY "../battle_server/src/PackMsg.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/PackMsg.cpp" DESTINATION "./tools/")

file(COPY "../battle_server/src/Redirect.cpp" DESTINATION "./tools/")


file(COPY "../battle_server/src/StatCount.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/StatCount.cpp" DESTINATION "./tools/")

file(COPY "../battle_server/src/ManageStat.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/ManageStat.cpp" DESTINATION "./tools/")

include_directories(./inc ./src ./tools ./protocol ../deps ../battle_server/inc ../battle_server/src ../battle_server/tools ../deps/netcore/inc ../deps/simplexml/inc ../deps/logger/inc $ENV{BOOST_INC} $ENV{EVENT_INC})
LINK_DIRECTORIES(../deps/libs $ENV{BOOST_LIB} $ENV{LEV_LIB} /open_source/RakNet-master/Lib/DLL)

aux_source_directory(./src SRC_FILE)
aux_source_directory(./tools TOOLS_FILE)

ADD_EXECUTABLE(gatesys ${SRC_FILE} ${TOOLS_FILE})

TARGET_LINK_LIBRARIES(gatesys simplexml)
TARGET_LINK_LIBRARIES(gatesys logger)
TARGET_LINK_LIBRARIES(gatesys event_core)
TARGET_LINK_LIBRARIES(gatesys netcore)
TARGET_LINK_LIBRARIES(gatesys boost_thread)
TARGET_LINK_LIBRARIES(gatesys boost_date_time)
TARGET_LINK_LIBRARIES(gatesys boost_filesystem)
TARGET_LINK_LIBRARIES(gatesys RakNetDLL)


