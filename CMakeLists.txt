cmake_minimum_required(VERSION 3.10)

project(UDPPacketTransfer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_COMP)


file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${PROJECT_SOURCE_DIR}/main.cpp)
