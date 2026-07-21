cmake_minimum_required(VERSION 3.20)

project(CEssentials C)

add_library(${PROJECT_NAME} STATIC
  "lib/test.c"
)

target_include_directories(${PROJECT_NAME} PUBLIC "include")

option(BUILD_TESTS "Build Tests" OFF)

if (BUILD_TESTS)
  add_subdirectory("test")
endif()
