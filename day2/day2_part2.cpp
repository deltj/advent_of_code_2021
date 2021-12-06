/**
 * Advent of Code 2021
 * Day 2 - Part 2
 * 
 * Given a list of movement instructions, track the horizontal and vertical 
 * position (depth) of a submarine.  This implementation differs from part 1
 * in that the up/down instructions modify an attack angle (aim) parameter
 * instead of depth.  Print out the value of the horizontal position
 * multiplied by the vertical position.
 */
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day2_part2 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string instruction;
    int horizontal_pos = 0;
    int depth = 0;
    int aim = 0;

    typedef std::vector<std::string> splitvec;

    //  Read the input file one line at a time
    while(std::getline(ifs, instruction))
    {
        std::cout << instruction << std::endl;

        splitvec tokens;
        boost::split(tokens, instruction, boost::is_any_of(" "));

        if(tokens.size() != 2) continue;

        const std::string direction = tokens[0];
        const int magnitude = boost::lexical_cast<int>(tokens[1]);

        if(direction == "forward")
        {
            horizontal_pos += magnitude;
            depth += magnitude * aim;
        }
        else if(direction == "up")
        {
            aim -= magnitude;
        }
        else if(direction == "down")
        {
            aim += magnitude;
        }

        std::cout << "horizontal position: " << horizontal_pos << std::endl;
        std::cout << "aim: " << aim << std::endl;
        std::cout << "depth: " << depth << std::endl;
    }
    ifs.close();

    std::cout << "Position product: " << horizontal_pos * depth << std::endl;

    return EXIT_SUCCESS;
}