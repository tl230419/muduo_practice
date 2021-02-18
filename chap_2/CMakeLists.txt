cmake_minimum_required(VERSION 2.8)

project(chap_2_1_1)

include_directories("../")

file(GLOB HEADERS 
    ${PROJECT_SOURCE_DIR}/*.h)

file(GLOB SRCS 
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/*.cc)

add_executable(chap_2_1_1 ${HEADERS} ${SRCS})