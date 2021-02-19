cmake_minimum_required(VERSION 2.8)

project(chap_2_8_4)

FIND_PACKAGE(Threads)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11")

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories("../../")

file(GLOB HEADERS 
    ${PROJECT_SOURCE_DIR}/*.h)

file(GLOB SRCS 
    ${PROJECT_SOURCE_DIR}/*.cc
    ${PROJECT_SOURCE_DIR}/../../*.cc)

add_executable(${PROJECT_NAME} ${HEADERS} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})