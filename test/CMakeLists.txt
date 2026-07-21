
add_executable(StringTest1
  "StringTest1.c"
)

target_link_libraries(StringTest1 PRIVATE CEssentials)

enable_testing()

add_test(
  NAME StringTest1
  COMMAND "StringTest1"
)
