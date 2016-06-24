cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ./src ../simplexml/inc ../flatbuffers $ENV{BOOST_INC})
aux_source_directory(./src SRC_FILE)
add_library(logger SHARED ${SRC_FILE})
#file(COPY "liblogger.so" DESTINATION "../libs")
