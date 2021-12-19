#ifndef LOTTERYBOARD_H
#define LOTTERYBOARD_H

#include <cstring>
#include <iostream>
#include <string>

class LotteryBoard
{
public:
    LotteryBoard(const size_t size=5) :
        boardSize(size),
        board(nullptr),
        marked(nullptr)
    {
        //  Allocate and zeroize board memory
        board = new int[size * size];
        memset(board, 0, size * size * sizeof(int));

        //  Allocate and zeroize cell status
        marked = new int[size * size];
        memset(marked, 0, size * size * sizeof(int));
    }

    LotteryBoard(const LotteryBoard &src) :
        boardSize(src.boardSize),
        board(nullptr),
        marked(nullptr)
    {
        //std::cout << "A lottery board has been copied! " << std::endl;

        board = new int[boardSize * boardSize];
        memcpy(board, src.board, boardSize * boardSize * sizeof(int));

        //  Allocate and zeroize cell status
        marked = new int[boardSize * boardSize];
        memcpy(marked, src.marked, boardSize * boardSize * sizeof(int));
    }

    virtual ~LotteryBoard()
    {
        //  Free board memory
        delete[] board;

        //  Free cell status
        delete[] marked;
    }

    //  Return the value of the specified cell
    int getCell(const int &row, const int &col) const
    {
        return board[((row - 1) * boardSize) + col - 1];
    }

    //  Set the value of the specified cell
    void setCell(const int &row, const int &col, const int &val)
    {
        board[((row - 1) * boardSize) + col - 1] = val;
    }

    //  Return true if the specified cell has been marked (the number has been called)
    bool isMarked(const int &row, const int &col) const
    {
        return marked[((row - 1) * boardSize) + col - 1] > 0;
    }

    //  Mark the specified cell (indicate that its number has been called)
    void mark(const int &row, const int &col)
    {
        marked[((row - 1) * boardSize) + col - 1] = 1;
    }

    //  Mark all cells having the specified value
    void markAll(const int &val)
    {
        for(int row=1; row<=boardSize; row++)
        {
            for(int col=1; col<=boardSize; col++)
            {
                if(getCell(row, col) == val)
                {
                    mark(row, col);
                }
            }
        }
    }

    bool check() const
    {
        //  Check for vertical wins
        for(int col=1; col<=boardSize; col++)
        {
            bool vwin = true;
            for(int row=1; row<=boardSize; row++)
            {
                if(!isMarked(row, col))
                {
                    vwin = false;
                    break;
                }
            }

            if(vwin)
            {
                std::cout << "vertical win on col " << col << std::endl;
                return true;
            }
        }

        //  Check for horizontal wins
        for(int row=1; row<=boardSize; row++)
        {
            bool hwin = true;
            for(int col=1; col<=boardSize; col++)
            {
                if(!isMarked(row, col))
                {
                    hwin = false;
                    break;
                }
            }

            if(hwin)
            {
                std::cout << "horizontal win on row " << row << std::endl;
                return true;
            }
        }

        return false;
    }

    bool checkWithDiagonal() const
    {
        if(check()) return true;

        //  Check for diagonal wins (top left to bottom right)
        bool dwin = true;
        for(int i=1; i<=boardSize; i++)
        {
            if(!isMarked(i, i))
            {
                dwin = false;
                break;
            }
        }
        if(dwin)
        {
            std::cout << "diagonal win (1)" << std::endl;
            return true;
        }

        //  Check for diagonal wins (top right to bottom left)
        dwin = true;
        for(int i=1; i<=boardSize; i++)
        {
            //std::cout << "(" << i << "," << boardSize - i + 1 << ")" << "=" << getCell(i, boardSize - i + 1) << std::endl;
            if(!isMarked(i, boardSize - i + 1))
            {
                dwin = false;
                break;
            }
        }
        if(dwin)
        {
            std::cout << "diagonal win (2)" << std::endl;
            return true;
        }

        return false;
    }

    void printBoard() const
    {
        for(int row=1; row<=boardSize; row++)
        {
            for(int col=1; col<=boardSize; col++)
            {
                std::cout << getCell(row, col) << " ";
            }
            std::cout << std::endl;
        }
    }

    int markedSum() const
    {
        int sum = 0;
        for(int row=1; row<=boardSize; row++)
        {
            for(int col=1; col<=boardSize; col++)
            {
                if(isMarked(row, col))
                {
                    sum += getCell(row, col);
                }
            }
        }
        return sum;
    }

    int unmarkedSum() const
    {
        int sum = 0;
        for(int row=1; row<=boardSize; row++)
        {
            for(int col=1; col<=boardSize; col++)
            {
                if(!isMarked(row, col))
                {
                    sum += getCell(row, col);
                }
            }
        }
        return sum;
    }

private:
    int boardSize;
    int *board;
    int *marked;
};

#endif