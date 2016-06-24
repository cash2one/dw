cmake_minimum_required(VERSION 2.8)

PROJECT(battlesys)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ./src ./tools ./protocol ../deps/ ../deps/netcore/inc ../deps/simplexml/inc ../deps/logger/inc ../deps/mysql++ ../deps/MySQL_5_1/include $ENV{BOOST_INC} $ENV{EVENT_INC} $ENV{LEV_INC})
LINK_DIRECTORIES(../deps/netcore ../deps/libs ../deps/logger ../deps/simplexml $ENV{BOOST_LIB} $ENV{TCM_LIB} $ENV{LEV_LIB} /open_source/RakNet-master/Lib/DLL /open_source/mysql++-3.2.2/)
aux_source_directory(./src SRC_FILE)
aux_source_directory(./tools TOOLS_FILE)
ADD_EXECUTABLE(battlesys ${SRC_FILE} ${TOOLS_FILE})
TARGET_LINK_LIBRARIES(battlesys simplexml)
#TARGET_LINK_LIBRARIES(battlesys tcmalloc)
TARGET_LINK_LIBRARIES(battlesys logger)
TARGET_LINK_LIBRARIES(battlesys event_core)
TARGET_LINK_LIBRARIES(battlesys netcore)
TARGET_LINK_LIBRARIES(battlesys boost_thread)
TARGET_LINK_LIBRARIES(battlesys boost_date_time)
TARGET_LINK_LIBRARIES(battlesys boost_filesystem)
TARGET_LINK_LIBRARIES(battlesys RakNetDLL)
TARGET_LINK_LIBRARIES(battlesys mysqlpp)
TARGET_LINK_LIBRARIES(battlesys mysqlclient)


