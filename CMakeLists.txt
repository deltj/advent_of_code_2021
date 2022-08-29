cmake_minimum_required(VERSION 3.0)

project(advent_of_code_2021)
enable_testing()

find_package(Boost 1.71 REQUIRED COMPONENTS regex unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")

add_executable(day1 day1.cpp)
target_link_libraries(day1 ${Boost_LIBRARIES})
add_test(day1 day1)

add_executable(day2 day2.cpp)
target_link_libraries(day2 ${Boost_LIBRARIES})
add_test(day2 day2)

add_executable(day3 day3.cpp)
target_link_libraries(day3 ${Boost_LIBRARIES})
add_test(day3 day3)

add_executable(day4 day4.cpp)
target_link_libraries(day4 ${Boost_LIBRARIES})
add_test(day4 day4)

add_executable(day5 day5.cpp)
target_link_libraries(day5 ${Boost_LIBRARIES})
add_test(day5 day5)

add_executable(day6 day6.cpp)
target_link_libraries(day6 ${Boost_LIBRARIES})
add_test(day6 day6)

add_executable(day7 day7.cpp)
target_link_libraries(day7 ${Boost_LIBRARIES})
add_test(day7 day7)