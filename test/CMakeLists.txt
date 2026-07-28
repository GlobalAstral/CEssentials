
add_executable(StringTest1
  "StringTest1.c"
)
target_link_libraries(StringTest1 PRIVATE CEssentials)

add_executable(StringTest2
  "StringTest2.c"
)
target_link_libraries(StringTest2 PRIVATE CEssentials)

add_executable(StringTest3
  "StringTest3.c"
)
target_link_libraries(StringTest3 PRIVATE CEssentials)

add_executable(ArrayListsTest1
  "ArrayListsTest1.c"
)
target_link_libraries(ArrayListsTest1 PRIVATE CEssentials)

add_executable(LinkedListsTest
  "LinkedListsTest.c"
)
target_link_libraries(LinkedListsTest PRIVATE CEssentials)


enable_testing()

add_test(
  NAME StringTest1
  COMMAND "StringTest1"
)

add_test(
  NAME StringTest2
  COMMAND "StringTest2"
)

add_test(
  NAME StringTest3
  COMMAND "StringTest3"
)

add_test(
  NAME ArrayListsTest1
  COMMAND "ArrayListsTest1"
)

add_test(
  NAME LinkedListsTest
  COMMAND "LinkedListsTest"
)
