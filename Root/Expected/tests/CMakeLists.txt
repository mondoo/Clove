add_executable(ExpectedTest ExpectedTests.cpp)
target_link_libraries(ExpectedTest PRIVATE gtest_main Root.Expected)
add_test(NAME ExpectedTest COMMAND ExpectedTest)