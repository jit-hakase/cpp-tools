cmake_minimum_required(VERSION 3.12)
project(DEMO-TEST)

set(CMAKE_CXX_STANDARD 11)
set(WORK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configuare_file(
	${WORK_DIR}/Version.h.in
	${WORK_DIR}/Version.h
)

#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g -O2 -Wall -rdynamic")

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

include_directories(include)

add_executable(test_demo source/demo.cpp)
#target_link_libraries(test_demo pthread)
