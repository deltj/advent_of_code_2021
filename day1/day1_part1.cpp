/**
 * Advent of Code 2021
 * Day 1 - Part 1
 * 
 * Given a list of numbers (sonar depth measurements), determine how many
 * measurements increased from the previous measurement. 
 */
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day1_part1 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string line;
    int depth_measurement = 0;
    int previous_depth_measurement = 0;
    int inc = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        previous_depth_measurement = depth_measurement;

        depth_measurement = boost::lexical_cast<int>(line);
        std::cout << depth_measurement << std::endl;

        //  Skip increasing/decreasing check for first measurement
        if(previous_depth_measurement == 0)
        {
            continue;
        }

        if(depth_measurement > previous_depth_measurement)
        {
            inc++;
        }
    }
    ifs.close();

    std::cout << "Number of increasing measurements: " << inc << std::endl;

    return EXIT_SUCCESS;
}