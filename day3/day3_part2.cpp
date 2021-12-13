/**
 * Advent of Code 2021
 * Day 3 - Part 1
 * 
 * Find oxygen sensor rating and CO2 scrubber rating as described in the problem description.
 */
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

/**
 * Find the most common value of the ith bit in the specified vector of ints.
 * 
 * @param v The vector to consider
 * @param i The bit to consider
 * @return The most common value (1 or 0) observed in the ith bit
 */
int most_common_bit(const std::vector<int> &v, const int &i)
{
    int bit_sum = 0;

    for(std::vector<int>::const_iterator it=v.begin(); it!=v.end(); ++it)
    {
        bit_sum += (*it >> i) & 1;
    }

    return bit_sum > (v.size() / 2);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day3_part2 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string line;

    std::vector<int> input;

    int num_bits = 0;

    std::cout << "Reading file: " << argv[1] << std::endl;

    //  Read the input numbers into a vector
    int number;
    while(std::getline(ifs, line))
    {
        //std::cout << line << std::endl;
        if(num_bits == 0)
        {
            num_bits = line.length();
            std::cout << "Bits per number: " << num_bits << std::endl;
        }

        if(line.length() != num_bits) continue;

        number = 0;
        for(int i=0; i<num_bits; i++)
        {
            const int bit = boost::lexical_cast<int>(line.at(i));
            number <<= 1;
            number |= bit;
        }

        //std::cout << number << std::endl;
        input.push_back(number);
    }
    ifs.close();

    std::cout << "Read " << input.size() << " numbers" << std::endl;

    std::vector<int> ogr_input = input;

    for(int bitnum=1; bitnum<=num_bits; bitnum++)
    {
        const int shift_amt = num_bits - bitnum;
        int ones = 0;
        int zeroes = 0;

        //  Find the most common value for this bit
        for(std::vector<int>::const_iterator it=ogr_input.begin(); it!=ogr_input.end(); ++it)
        {
            const int bit = (*it >> shift_amt) & 1;

            if(bit == 1) ones++;
            if(bit == 0) zeroes++;
        }
        const int mcv = ones >= zeroes ? 1 : 0;
        std::cout << "bit " << bitnum << " mcv: " << mcv << std::endl;

        //  For OGR, remove numbers that do not have the most common value for this bit
        auto new_ogr_end = std::remove_if(ogr_input.begin(), ogr_input.end(),
            [shift_amt, mcv](int num) { return ((num >> shift_amt) & 1) != mcv; });
        ogr_input.erase(new_ogr_end, ogr_input.end());

        std::cout << "Filtered OGR input size: " << ogr_input.size() << std::endl;

        if(ogr_input.size() == 1)
        {
            std::cout << "OGR value: " << ogr_input[0] << std::endl;
            break;
        }
    }
    
    std::vector<int> csr_input = input;

    for(int bitnum=1; bitnum<=num_bits; bitnum++)
    {
        const int shift_amt = num_bits - bitnum;
        int ones = 0;
        int zeroes = 0;

        //  Find the most common value for this bit
        for(std::vector<int>::const_iterator it=csr_input.begin(); it!=csr_input.end(); ++it)
        {
            const int bit = (*it >> shift_amt) & 1;

            if(bit == 1) ones++;
            if(bit == 0) zeroes++;
        }
        const int lcv = zeroes <= ones ? 0 : 1;
        std::cout << "bit " << bitnum << " lcv: " << lcv << std::endl;

        //  For CSR, remove numbers that do not have the least common value for this bit
        auto new_csr_end = std::remove_if(csr_input.begin(), csr_input.end(),
            [shift_amt, lcv](int num) { return ((num >> shift_amt) & 1) != lcv; });
        csr_input.erase(new_csr_end, csr_input.end());

        std::cout << "Filtered CSR input size: " << csr_input.size() << std::endl;

        if(csr_input.size() == 1)
        {
            std::cout << "CSR value: " << csr_input[0] << std::endl;
            break;
        }
    }

    const int lsr = ogr_input[0] * csr_input[0];
    std::cout << "LSR: " << lsr << std::endl;
    
    return EXIT_SUCCESS;
}