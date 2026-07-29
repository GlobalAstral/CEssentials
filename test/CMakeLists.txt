
add_executable(Main
  "Main.c"
)
target_link_libraries(Main PRIVATE CEssentials)

enable_testing()

add_test(
  NAME StringTest1
  COMMAND Main string1
)

add_test(
  NAME StringTest2
  COMMAND Main string2
)

add_test(
  NAME StringTest3
  COMMAND Main string3
)

add_test(
  NAME ArrayListsTest
  COMMAND Main arraylist
)

add_test(
  NAME LinkedListsTest
  COMMAND Main linkedlist
)

add_test(
  NAME StackTest
  COMMAND Main stack
)

add_test(
  NAME BitFieldTest
  COMMAND Main bitfield
)

add_test(
  NAME BitArrayTest
  COMMAND Main bitarray
)

add_test(
  NAME LoggerTest
  COMMAND Main logger
)
