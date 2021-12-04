find_package(GTest REQUIRED CONFIG)

add_executable(ExpectedTest ExpectedTests.cpp)
target_link_libraries(ExpectedTest PRIVATE GTest::gtest CloveExpected)
add_test(NAME ExpectedTest COMMAND ExpectedTest)