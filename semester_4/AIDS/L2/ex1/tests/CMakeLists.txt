add_executable(
  quick_sort_test
  quick_sort_test.cpp
)

target_link_libraries(
  quick_sort_test
  gtest_main
)

add_executable(
  insertion_sort_test
  insertion_sort_test.cpp
)

target_link_libraries(
  insertion_sort_test
  gtest_main
)

add_executable(
  hybrid_sort_test
  hybrid_sort_test.cpp
)

target_link_libraries(
  hybrid_sort_test
  gtest_main
)

add_executable(
  dual_pivot_quicksort_test
  dual_pivot_quicksort_test.cpp
)

target_link_libraries(
  dual_pivot_quicksort_test
  gtest_main
)

add_executable(
  merge_sorts_test
  merge_sorts_test.cpp
)

target_link_libraries(
  merge_sorts_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(quick_sort_test)
gtest_discover_tests(insertion_sort_test)
gtest_discover_tests(hybrid_sort_test)
gtest_discover_tests(dual_pivot_quicksort_test)
gtest_discover_tests(merge_sorts_test)