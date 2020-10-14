add_executable(EventTest EventTests.cpp)
target_link_libraries(EventTest PRIVATE gtest_main Root.Event)
add_test(NAME EventTest COMMAND EventTest)