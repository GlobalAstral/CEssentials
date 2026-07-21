
add_executable(Test
  "Test.c"
)

target_link_libraries(Test PRIVATE CEssentials)

enable_testing()

add_test(
  NAME Test
  COMMAND "Test"
)
