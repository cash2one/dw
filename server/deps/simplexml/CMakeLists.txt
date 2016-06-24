cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11")
#add_definitions(-DBOOST_ASIO_DISABLE_MOVE=1)

include_directories(./inc ./src ./tinyxml ../share/inc $ENV{BOOST_INC})
aux_source_directory(./src SRC_FILE)
aux_source_directory(./tinyxml TINYXML_FILE)
add_library(simplexml SHARED ${SRC_FILE} ${TINYXML_FILE})
#file(COPY "libsimplexml.so" DESTINATION "../libs")