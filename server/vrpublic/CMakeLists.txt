cmake_minimum_required(VERSION 2.8)

PROJECT(vrpsys)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

file(COPY "../battle_server/protocol/guid_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/protocol_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/inter_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/login_generated.h" DESTINATION "./tools/")
file(COPY "../battle_server/protocol/msg_error_generated.h" DESTINATION "./tools/")

file(COPY "../guidsys/inc/MakeGuid.h" DESTINATION "./tools/")
file(COPY "../guidsys/inc/MakeGuid.cpp" DESTINATION "./tools/")

file(COPY "../deps/entity/entity/vr/account.pb.h" DESTINATION "./tools/")
file(COPY "../deps/entity/entity/vr/account.pb.cc" DESTINATION "./tools/")
file(COPY "../deps/entity/entity/vr/cache_auto_assistant.h" DESTINATION "./tools/")
file(COPY "../deps/entity/entity/vr/cache_auto_assistant.cpp" DESTINATION "./tools/")

file(COPY "../battle_server/tools/Packet_.h" DESTINATION "./tools/")
file(COPY "../battle_server/tools/Packet_.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/Handle.h" DESTINATION "./tools/")

file(COPY "../battle_server/tools/ManageSingleton.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/MessagePump.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/PacketHandle.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/Singleton.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/tools/ManageDbPing.cpp" DESTINATION "./tools/")
file(COPY "../battle_server/src/Redirect.cpp" DESTINATION "./tools/")


file(COPY "../battle_server/src/PackMsg.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/PackMsg.cpp" DESTINATION "./tools/")


file(COPY "../battle_server/src/StatCount.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/StatCount.cpp" DESTINATION "./tools/")

file(COPY "../battle_server/src/ManageStat.h" DESTINATION "./tools/")
file(COPY "../battle_server/src/ManageStat.cpp" DESTINATION "./tools/")

include_directories(./inc ./src ./tools ./protocol ../deps ../deps/entity ../deps/protobuf ../deps/mysql++ ../deps/MySQL_5_1/include ../battle_server/inc ../battle_server/src ../battle_server/tools ../deps/netcore/inc ../deps/simplexml/inc ../deps/logger/inc $ENV{BOOST_INC} $ENV{EVENT_INC})
LINK_DIRECTORIES(../deps/libs $ENV{BOOST_LIB} $ENV{LEV_LIB} /open_source/RakNet-master/Lib/DLL /open_source/mysql++-3.2.2/ /open_source/protobuf-2.6.1/src/.libs/)

aux_source_directory(./src SRC_FILE)
aux_source_directory(./tools TOOLS_FILE)

ADD_EXECUTABLE(vrpsys ${SRC_FILE} ${TOOLS_FILE})

TARGET_LINK_LIBRARIES(vrpsys simplexml)
TARGET_LINK_LIBRARIES(vrpsys logger)
TARGET_LINK_LIBRARIES(vrpsys event_core)
TARGET_LINK_LIBRARIES(vrpsys netcore)
TARGET_LINK_LIBRARIES(vrpsys boost_thread)
TARGET_LINK_LIBRARIES(vrpsys boost_date_time)
TARGET_LINK_LIBRARIES(vrpsys boost_filesystem)
TARGET_LINK_LIBRARIES(vrpsys RakNetDLL)
TARGET_LINK_LIBRARIES(vrpsys mysqlpp)
TARGET_LINK_LIBRARIES(vrpsys protobuf)
TARGET_LINK_LIBRARIES(vrpsys mysqlclient)



