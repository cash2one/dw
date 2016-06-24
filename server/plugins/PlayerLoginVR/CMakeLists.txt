cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ./src ../../deps/logger/inc ../../deps/plugin/inc ../../deps/pool/inc ../../deps ../../battle_server/protocol ../../deps ../../battle_server/tools ../../battle_server/src ../../deps/netcore/inc ../../deps/protobuf ../../deps/entity/entity/vr $ENV{BOOST_INC})
aux_source_directory(./src SRC_FILE)

add_library(plugin_player_login_vr SHARED ${SRC_FILE} )
#file(COPY "libsimplexml.so" DESTINATION "../libs")