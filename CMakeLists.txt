cmake_minimum_required(VERSION 3.20)

project(CEssentials C)

add_library(${PROJECT_NAME} STATIC
  "lib/ce_string.c"
  "lib/ce_arraylist.c"
  "lib/ce_iterator.c"
  "lib/ce_linkedlist.c"
)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(${PROJECT_NAME} PRIVATE "private")

option(BUILD_TESTS "Build Tests" OFF)

if (BUILD_TESTS)
  add_subdirectory("test")
endif()
