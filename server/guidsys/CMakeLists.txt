cmake_minimum_required(VERSION 2.8)

PROJECT(guidsys)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

file(COPY "../battle_server/protocol/guid_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/protocol_generated.h" DESTINATION "./tools/")

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

ADD_EXECUTABLE(guidsys ${SRC_FILE} ${TOOLS_FILE})

TARGET_LINK_LIBRARIES(guidsys simplexml)
TARGET_LINK_LIBRARIES(guidsys logger)
TARGET_LINK_LIBRARIES(guidsys event_core)
TARGET_LINK_LIBRARIES(guidsys netcore)
TARGET_LINK_LIBRARIES(guidsys boost_thread)
TARGET_LINK_LIBRARIES(guidsys boost_date_time)
TARGET_LINK_LIBRARIES(guidsys boost_filesystem)
TARGET_LINK_LIBRARIES(guidsys RakNetDLL)


