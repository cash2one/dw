cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ./src ../simplexml/inc ../logger/inc ../protobuf ../boost ../mysql++ ../MySQL_5_1/include ../logger/inc ../utility/inc)
aux_source_directory(./src SRC_FILE)
add_library(dynamicparse SHARED ${SRC_FILE})
#file(COPY "libdynamicparse.so" DESTINATION "../libs")