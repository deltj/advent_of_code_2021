cmake_minimum_required(VERSION 3.0)

project(advent_of_code_2021)

find_package(Boost 1.71 COMPONENTS regex unit_test_framework)

set(CMAKE_CXX_FLAGS "-Wall -g")

add_executable(day1_part1 day1/day1_part1.cpp)
add_executable(day1_part2 day1/day1_part2.cpp)

add_executable(day2_part1 day2/day2_part1.cpp)
add_executable(day2_part2 day2/day2_part2.cpp)

add_executable(day3_part1 day3/day3_part1.cpp)
add_executable(day3_part2 day3/day3_part2.cpp)

add_executable(lb_test day4/LotteryBoard_test.cpp)
target_link_libraries(lb_test
    boost_unit_test_framework)
add_executable(day4_part1 day4/day4_part1.cpp)