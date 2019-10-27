cmake_minimum_required(VERSION 2.6)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -pedantic ${CMAKE_CXX_FLAGS}")
endif()

include_directories(src src/castor/src src/castor/src/SANDAL2/src)
add_subdirectory(src/castor)


file(GLOB_RECURSE ${CMAKE_PROJECT_NAME}_sources src/${CMAKE_PROJECT_NAME}/*.cpp)
add_executable( ${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_sources})
target_link_libraries(${CMAKE_PROJECT_NAME}
  sandal2
  castor
  )

file(GLOB_RECURSE testing_sources src/testing/*.cpp)

foreach(test_source ${testing_sources})
  get_filename_component(test_exec ${test_source} NAME_WE)
  add_executable(${test_exec} ${test_source})
endforeach(test_source ${testing_sources})
