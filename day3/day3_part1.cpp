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

    //  I'm using a vector to store the sum of all bits by index
    //  (so bit_sum[i] contains the sum of the ith bits from each number)
    std::vector<int> bit_sum;
    
    //  zeroize the sum to start
    for(int i=0; i<12; i++)
    {
        bit_sum.push_back(0);
    }

    //  Keep track of the number of lines read in.  If the bit_sum for position i
    //  is greater than number of lines / 2, that bit should be a 1 in the gamma rate
    //  (and a zero in the epsilon rate)
    int num_lines = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        std::cout << line << std::endl;
        num_lines++;

        if(line.length() != 12) continue;

        //  Consider each bit in the input and update the bit sums
        for(int i=0; i<12; i++)
        {
            int bit = boost::lexical_cast<int>(line.at(i));
            bit_sum[i] += bit;
        }
    }
    ifs.close();

    std::cout << "lines: " << num_lines << std::endl;
    std::cout << "bit sums: ";
    int gamma_rate = 0;
    for(int i=0; i<12; i++)
    {
        int bit = bit_sum[i] > (num_lines / 2) ? 1 : 0;
        gamma_rate <<= 1;
        gamma_rate |= bit;
        
        std::cout << bit_sum[i] << "(" << bit << ") "; 
    }
    std::cout << std::endl;

    std::cout << "gamma rate: " << gamma_rate << std::endl;

    int epsilon_rate = (~gamma_rate) & 0xfff;
    std::cout << "epsilon rate: " << epsilon_rate << std::endl;

    int power_consumption = gamma_rate * epsilon_rate;
    std::cout << "power consumption: " << power_consumption << std::endl;

    return EXIT_SUCCESS;
}