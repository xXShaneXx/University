add_executable(
  quick_sort_test
  quick_sort_test.cpp
)

target_link_libraries(
  quick_sort_test
  gtest_main
  comparer
)

add_executable(
  insertion_sort_test
  insertion_sort_test.cpp
)

target_link_libraries(
  insertion_sort_test
  gtest_main
  comparer
)

include(GoogleTest)
gtest_discover_tests(quick_sort_test)
gtest_discover_tests(insertion_sort_test)