cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ../protobuf ../boost ../mysql++ ../MySQL_5_1/include ../dynamicparse/inc ../logger/inc ../utility/inc)
aux_source_directory(./src SRC_FILE)
add_library(pool SHARED ${SRC_FILE})
#file(COPY "libpool.so" DESTINATION "../libs")