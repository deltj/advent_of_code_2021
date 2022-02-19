
#define BOOST_TEST_MODULE Day4
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <set>
#include <string>
#include <vector>

class BingoBoardCell
{
public:
    BingoBoardCell() : value(0), marked(false) {}

    int value;
    bool marked;
};

class BingoBoard
{
public:
    BingoBoard(const size_t size=5) :
        boardSize(size),
        board(nullptr)
    {
        //  Allocate and board memory
        board = new BingoBoardCell[boardSize * boardSize];
    }

    BingoBoard(const BingoBoard &src) :
        boardSize(src.boardSize),
        board(nullptr)
    {
        board = new BingoBoardCell[boardSize * boardSize];

        for(int i=0; i<(boardSize * boardSize); i++)
        {
            board[i] = src.board[i];
        }
    }

    BingoBoard operator=(const BingoBoard &rhs)
    {
        boardSize = rhs.boardSize;

        if(board != nullptr)
        {
            delete[] board;
        }

        board = new BingoBoardCell[boardSize * boardSize];

        for(int i=0; i<(boardSize * boardSize); i++)
        {
            board[i] = rhs.board[i];
        }

        return *this;
    }

    virtual ~BingoBoard()
    {
        if(board)
        {
            //  Free board memory
            delete[] board;
        }
    }

    //  Return the value of the specified cell
    int getCell(const int &row, const int &col) const
    {
        return board[(row * boardSize) + col].value;
    }

    //  Set the value of the specified cell
    void setCell(const int &row, const int &col, const int &val)
    {
        board[(row * boardSize) + col].value = val;
    }

    //  Return true if the specified cell has been marked (the number has been called)
    bool isMarked(const int &row, const int &col) const
    {
        return board[(row * boardSize) + col].marked;
    }

    //  Mark the specified cell (indicate that its number has been called)
    void mark(const int &row, const int &col)
    {
        board[(row * boardSize) + col].marked = true;
    }

    //  Mark all cells having the specified value
    void markAll(const int &val)
    {
        for(int row=0; row<boardSize; row++)
        {
            for(int col=0; col<boardSize; col++)
            {
                if(getCell(row, col) == val)
                {
                    mark(row, col);
                }
            }
        }
    }

    void printBoard() const
    {
        for(int row=0; row<boardSize; row++)
        {
            for(int col=0; col<boardSize; col++)
            {
                std::cout << getCell(row, col) << " ";
            }
            std::cout << std::endl;
        }
    }

    //  Check for horizontal win
    bool checkForHorizontalWin() const
    {
        for(int row=0; row<boardSize; row++)
        {
            bool hwin = true;
            for(int col=0; col<boardSize; col++)
            {
                if(!isMarked(row, col))
                {
                    hwin = false;
                    break;
                }
            }

            if(hwin)
            {
                //std::cout << "horizontal win on row " << row << std::endl;
                return true;
            }
        }

        return false;
    }

    //  Check for vertical win
    bool checkForVerticalWin() const
    {
        for(int col=0; col<boardSize; col++)
        {
            bool vwin = true;
            for(int row=0; row<boardSize; row++)
            {
                if(!isMarked(row, col))
                {
                    vwin = false;
                    break;
                }
            }

            if(vwin)
            {
                //std::cout << "vertical win on col " << col << std::endl;
                return true;
            }
        }

        return false;
    }

    //  Find the sum of all unmarked cells
    int unmarkedSum() const
    {
        int sum = 0;
        for(int row=0; row<boardSize; row++)
        {
            for(int col=0; col<boardSize; col++)
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
    BingoBoardCell *board;
};

class BingoGame
{
public:
    BingoGame() {}

    void readBingoFile(std::ifstream &ifs)
    {
        std::string line;
        std::vector<std::string> tokens;

        //  The first line of input lists the bingo numbers
        std::getline(ifs, line);
        boost::split(tokens, line, boost::is_any_of(","));
        for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
        {
            numbers.push_back(boost::lexical_cast<int>(*it));
        }

        //  Subsequent lines define a number of bingo boards
        const int board_size = 5;
        int row = 0;
        int col = 0;
        BingoBoard tmpboard(board_size);
        while(std::getline(ifs, line))
        {
            if(line.length() == 0) continue;
            
            tokens.clear();
            boost::split(tokens, line, boost::is_any_of(" "));
            if(tokens.size() < board_size) 
            {
                std::cout << "something bad happened " << tokens.size() << std::endl;
                ifs.close();
                return;
            }

            col = 0;
            for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
            {
                if(it->length() == 0) continue;  //  deal with empty tokens from double spaces

                //std::cout << "(" << row << "," << col << ") cell value:" << *it << std::endl;
                tmpboard.setCell(row, col, boost::lexical_cast<int>(*it));

                col++;
            }

            row++;

            if(row == board_size)
            {
                boards.push_back(tmpboard);
                //tmpboard.printBoard();
                row = 0;
            }
        }
    }

    void play()
    {
        winningBoards.clear();

        std::vector<BingoBoard> boardsInPlay = boards;

        //  Play the bingo numbers in the order we received them
        for(std::vector<int>::const_iterator number_iter=numbers.begin(); number_iter!=numbers.end(); ++number_iter)
        {
            bool at_least_one_board_won = false;

            //  Check each board
            for(std::vector<BingoBoard>::iterator board_iter=boardsInPlay.begin(); board_iter!=boardsInPlay.end();)
            {
                //  Mark all the cells with this number
                board_iter->markAll(*number_iter);

                //  Check if this board is winning
                if(board_iter->checkForHorizontalWin() || board_iter->checkForVerticalWin())
                {
                    //  Add this board to the winning board vector to capture the win order
                    winningBoards.push_back(*board_iter);
                    
                    //  Remove this board from the boards in play, so we don't check it again
                    board_iter = boardsInPlay.erase(board_iter);

                    at_least_one_board_won = true;
                }
                else
                {
                    board_iter++;
                }
            }

            if(at_least_one_board_won) winningNumbers.push_back(*number_iter);
        }
    }

    size_t getNumberCount() const { return numbers.size(); }
    size_t getBoardCount() const { return boards.size(); }

    BingoBoard getFirstWinningBoard() const
    {
        if(winningBoards.size() > 0) return winningBoards[0];
        else return BingoBoard();
    }

    int getFirstWinningNumber() const
    {
        if(winningNumbers.size() > 0) return winningNumbers[0];
        else return -1;
    }

    int getFirstWinningScore() const { return getFirstWinningBoard().unmarkedSum() * getFirstWinningNumber(); }

    BingoBoard getLastWinningBoard() const
    {
        if(winningBoards.size() > 0) return winningBoards[winningBoards.size() - 1];
        else return BingoBoard();
    }

    int getLastWinningNumber() const
    {
        if(winningNumbers.size() > 0) return winningNumbers[winningNumbers.size() - 1];
        else return -1;
    }

    int getLastWinningScore() const { return getLastWinningBoard().unmarkedSum() * getLastWinningNumber(); }

private:
    std::vector<int> numbers;
    std::vector<BingoBoard> boards;

    std::vector<int> winningNumbers;
    std::vector<BingoBoard> winningBoards;
};

BOOST_AUTO_TEST_CASE( BingoBoard_ctor )
{
    BingoBoard b(5);
}

BOOST_AUTO_TEST_CASE( BingoBoard_getset )
{
    BingoBoard b(5);

    b.setCell(0, 0, 4321);
    b.setCell(4, 4, 1234);
    
    BOOST_CHECK_EQUAL(4321, b.getCell(0, 0));
    BOOST_CHECK_EQUAL(1234, b.getCell(4, 4));
}

BOOST_AUTO_TEST_CASE( BingoBoard_copy )
{
    BingoBoard b1(5);

    b1.setCell(0, 0, 4321);
    b1.setCell(4, 4, 1234);

    BingoBoard b2 = b1;
    
    BOOST_CHECK_EQUAL(4321, b2.getCell(0, 0));
    BOOST_CHECK_EQUAL(1234, b2.getCell(4, 4));
}

BOOST_AUTO_TEST_CASE( BingoBoard_mark_isMarked )
{
    BingoBoard b(10);

    for(int row=0; row<10; row++)
    {
        for(int col=0; col<10; col++)
        {
            b.mark(row, col);
            BOOST_CHECK_EQUAL(true, b.isMarked(row, col));
        }
    }
}

BOOST_AUTO_TEST_CASE( BingoBoard_markall1 )
{
    BingoBoard b(10);

    b.setCell(1, 1, 12345);

    b.markAll(12345);

    BOOST_CHECK_EQUAL(true, b.isMarked(1, 1));
}

BOOST_AUTO_TEST_CASE( BingoBoard_markall2 )
{
    BingoBoard b(10);

    b.setCell(1, 1, 12345);
    b.setCell(2, 2, 12345);
    b.setCell(3, 3, 12345);

    b.markAll(12345);

    BOOST_CHECK_EQUAL(true, b.isMarked(1, 1));
    BOOST_CHECK_EQUAL(true, b.isMarked(2, 2));
    BOOST_CHECK_EQUAL(true, b.isMarked(3, 3));
}

BOOST_AUTO_TEST_CASE( BingoBoard_win_horizontal )
{
    BingoBoard b(3);

    b.setCell(0, 0, 12345);
    b.setCell(0, 1, 12345);
    b.setCell(0, 2, 12345);

    b.markAll(12345);

    BOOST_CHECK_EQUAL(true, b.checkForHorizontalWin());
}

BOOST_AUTO_TEST_CASE( BingoBoard_win_vertical )
{
    BingoBoard b(3);

    b.setCell(0, 0, 12345);
    b.setCell(1, 0, 12345);
    b.setCell(2, 0, 12345);

    b.markAll(12345);

    BOOST_CHECK_EQUAL(true, b.checkForVerticalWin());
}


BOOST_AUTO_TEST_CASE( day4_part1_example )
{
    BingoGame game;

    //  Test with example input
    std::ifstream ifs("../day4_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    game.readBingoFile(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(27, game.getNumberCount());
    BOOST_CHECK_EQUAL(3, game.getBoardCount());

    game.play();

    BOOST_CHECK_EQUAL(24, game.getFirstWinningNumber());
    BOOST_CHECK_EQUAL(188, game.getFirstWinningBoard().unmarkedSum());
    BOOST_CHECK_EQUAL(4512, game.getFirstWinningScore());
}

BOOST_AUTO_TEST_CASE( day4_part1_actual )
{
    BingoGame game;

    //  Test with real input
    std::ifstream ifs("../day4_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    game.readBingoFile(ifs);

    ifs.close();

    game.play();

    BOOST_CHECK_EQUAL(63552, game.getFirstWinningScore());
}

BOOST_AUTO_TEST_CASE( day4_part2_example )
{
    BingoGame game;

    //  Test with example input
    std::ifstream ifs("../day4_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    game.readBingoFile(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(27, game.getNumberCount());
    BOOST_CHECK_EQUAL(3, game.getBoardCount());

    game.play();

    BOOST_CHECK_EQUAL(24, game.getFirstWinningNumber());
    BOOST_CHECK_EQUAL(188, game.getFirstWinningBoard().unmarkedSum());
    BOOST_CHECK_EQUAL(4512, game.getFirstWinningScore());

    BOOST_CHECK_EQUAL(13, game.getLastWinningNumber());
    BOOST_CHECK_EQUAL(148, game.getLastWinningBoard().unmarkedSum());
    BOOST_CHECK_EQUAL(1924, game.getLastWinningScore());
}

BOOST_AUTO_TEST_CASE( day4_part2_actual )
{
    BingoGame game;

    //  Test with real input
    std::ifstream ifs("../day4_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    game.readBingoFile(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(100, game.getNumberCount());
    BOOST_CHECK_EQUAL(100, game.getBoardCount());

    game.play();

    BOOST_CHECK_EQUAL(96, game.getFirstWinningNumber());
    BOOST_CHECK_EQUAL(662, game.getFirstWinningBoard().unmarkedSum());
    BOOST_CHECK_EQUAL(63552, game.getFirstWinningScore());

    BOOST_CHECK_EQUAL(20, game.getLastWinningNumber());
    BOOST_CHECK_EQUAL(451, game.getLastWinningBoard().unmarkedSum());
    BOOST_CHECK_EQUAL(9020, game.getLastWinningScore());
}