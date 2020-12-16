#System
add_executable(ECSSystemTest SystemTests.cpp)
target_link_libraries(ECSSystemTest PRIVATE gtest_main CloveECS)
add_test(NAME ECSSystemTest COMMAND ECSSystemTest)