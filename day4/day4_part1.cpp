/**
 * Advent of Code 2021
 * Day 4 - Part 1
 * 
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "LotteryBoard.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: day4_part1 <input>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream ifs(argv[1], std::ifstream::in);
    std::string line;

    //  The first line of input lists the lottery numbers
    std::vector<int> lottery_numbers;

    //  Subsequent lines define a number of lottery boards
    std::vector<LotteryBoard> boards;

    //  Read the first line and update the list of lottery numbers
    if(!std::getline(ifs, line))
    {
        std::cout << "something bad happened" << std::endl;
        ifs.close();
        return EXIT_FAILURE;
    }

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(","));
    for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
    {
        lottery_numbers.push_back(boost::lexical_cast<int>(*it));
    }
    std::cout << "read " << lottery_numbers.size() << " lottery numbers" << std::endl;

    //  Read boards
    const int board_size = 5;
    int row = 1;
    int col = 1;
    LotteryBoard tmpboard(board_size);
    while(std::getline(ifs, line))
    {
        if(line.length() == 0) continue;
        
        tokens.clear();
        boost::split(tokens, line, boost::is_any_of(" "));
        if(tokens.size() < board_size) 
        {
            std::cout << "something bad happened " << tokens.size() << std::endl;
            ifs.close();
            return EXIT_FAILURE;
        }

        col = 1;
        for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
        {
            if(it->length() == 0) continue;  //  deal with empty tokens from double spaces

            //std::cout << "(" << row << "," << col << ") cell value:" << *it << std::endl;
            tmpboard.setCell(row, col, boost::lexical_cast<int>(*it));

            col++;
        }

        row++;

        if(row > board_size)
        {
            boards.push_back(tmpboard);
            //tmpboard.printBoard();
            row = 1;
        }
    }
    ifs.close();

    std::cout << "read " << boards.size() << " boards" << std::endl;

    //  Print the boards
    for(std::vector<LotteryBoard>::iterator board_iter=boards.begin(); board_iter!=boards.end(); ++board_iter)
    {
        board_iter->printBoard();
        std::cout << std::endl;
    }

    std::cout << "starting the game" << std::endl;

    //  Iterate over lottery numbers and mark the boards
    for(std::vector<int>::const_iterator it=lottery_numbers.begin(); it!=lottery_numbers.end(); ++it)
    {
        std::cout << "considering " << *it << std::endl;

        for(std::vector<LotteryBoard>::iterator board_iter=boards.begin(); board_iter!=boards.end(); ++board_iter)
        {
            //  Mark all the cells with this lottery number
            board_iter->markAll(*it);

            //  Check if this board is winning
            if(board_iter->check())
            {
                std::cout << "This board is winning:" << std::endl;
                board_iter->printBoard();

                std::cout << "Winning board sum is: " << board_iter->unmarkedSum() << std::endl;

                std::cout << "Final score is: " << board_iter->unmarkedSum() * *it << std::endl;
                return EXIT_SUCCESS;
            }
        }
    }

    return EXIT_SUCCESS;
}