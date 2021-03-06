cmake_minimum_required(VERSION 3.12)
project(DEMO-TEST)

set(CMAKE_CXX_STANDARD 11)
set(WORK_DIR ${CMAKE_CURRENT_SOURCE_DIR})

execute_process(
	COMMAND git log --pretty=oneline
	COMMAND wc -l
	COMMAND tr -d '\n'
	OUTPUT_VARIABLE COMMIT_NUMBER
)

execute_process(
	COMMAND git log --pretty=oneline --abbrev-commit
	COMMAND head -1
	COMMAND cut -d " " -f 1
	COMMAND tr -d '\n'
	OUTPUT_VARIABLE COMMIT_HASH
)

set(VERSION ${COMMIT_NUMBER}.${COMMIT_HASH})

configure_file(
	${WORK_DIR}/include/Version.h.in
	${WORK_DIR}/include/Version.h
)

option(DEBUG ON)

if (DEBUG)
	add_definitions(-D DEBUG)
	set(CMAKE_BUILD_TYPE "Debug")
	set(CMAKE_CXX_FLAGS_RELEASE "ENV{CXXFLAGS} -g -O0 -Wall -rdynamic")
else ()
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_FLAGS_RELEASE "ENV{CXXFLAGS} -O2 -Wall")
endif ()

include_directories(include)

add_executable(test_demo source/demo.cpp)
#target_link_libraries(test_demo pthread)
