/**
 * Advent of Code 2021
 * Day 1 - Part 2
 * 
 * Given a list of numbers (sonar depth measurements), determine how often the
 * measurements are increasing using a three-measurement sliding window.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>

#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day1_part2 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string line;
    int depth_measurement = 0;
    int window_sum = 0;
    int previous_window_sum = 0;
    int inc = 0;
    std::list<int> window;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        depth_measurement = boost::lexical_cast<int>(line);
        //std::cout << "measurement: " << depth_measurement << std::endl;

        window.push_front(depth_measurement);

        if(window.size() != 3)
        {
            continue;
        }

        previous_window_sum = window_sum;
        window_sum = std::accumulate(window.begin(), window.end(), 0);
        //std::cout << "window: " << window_sum << std::endl;

        window.pop_back();

        if(previous_window_sum == 0)
        {
            continue;
        }

        if(window_sum > previous_window_sum)
        {
            inc++;
        }
    }
    ifs.close();

    std::cout << "Number of increasing measurements: " << inc << std::endl;

    return EXIT_SUCCESS;
}