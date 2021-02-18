cmake_minimum_required(VERSION 2.8)

project(chap_2_1_1)

FIND_PACKAGE(Threads)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11")

include_directories("../../")

file(GLOB HEADERS 
    ${PROJECT_SOURCE_DIR}/*.h)

file(GLOB SRCS 
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/../../*.cc)

add_executable(${PROJECT_NAME} ${HEADERS} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})