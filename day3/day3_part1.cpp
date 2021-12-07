/**
 * Advent of Code 2021
 * Day 3 - Part 1
 * 
 * Given a list of binary numbers, all having the same number of bits, find
 * the numbers represented by the most and least common values for each bit
 * position.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day3_part1 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string line;
    std::vector<int> bitsum;
    
    for(int i=0; i<12; i++)
    {
        bitsum.push_back(0);
    }

    int num_lines = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        std::cout << line << std::endl;
        num_lines++;

        if(line.length() != 12) continue;

        for(int i=0; i<12; i++)
        {
            int bit = boost::lexical_cast<int>(line.at(i));
            bitsum[i] += bit;
        }
    }
    ifs.close();

    std::cout << "lines: " << num_lines << std::endl;
    std::cout << "bitsum: ";
    for(int i=0; i<12; i++)
    {
        std::cout << bitsum[i] << " "; 
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}