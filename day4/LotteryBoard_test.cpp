#include "LotteryBoard.h"

#define BOOST_TEST_MODULE LotteryBoard
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( LotteryBoard_ctor )
{
    LotteryBoard lb(10);

    for(int row=1; row<=10; row++)
    {
        for(int col=1; col<=10; col++)
        {
            BOOST_CHECK_EQUAL(0, lb.getCell(row, col));
        }
    }

    for(int row=1; row<=10; row++)
    {
        for(int col=1; col<=10; col++)
        {
            BOOST_CHECK_EQUAL(false, lb.isMarked(row, col));
        }
    }
}

BOOST_AUTO_TEST_CASE( LotteryBoard_copyctor )
{
    LotteryBoard lb1(3);

    lb1.setCell(1, 1, 1);
    lb1.setCell(2, 2, 2);
    lb1.setCell(3, 3, 3);

    LotteryBoard lb2(lb1);

    BOOST_CHECK_EQUAL(1, lb2.getCell(1, 1));
    BOOST_CHECK_EQUAL(2, lb2.getCell(2, 2));
    BOOST_CHECK_EQUAL(3, lb2.getCell(3, 3));
}

BOOST_AUTO_TEST_CASE( LotteryBoard_getset )
{
    LotteryBoard lb(10);

    for(int row=1; row<=10; row++)
    {
        for(int col=1; col<=10; col++)
        {
            lb.setCell(row, col, 12345);
            BOOST_CHECK_EQUAL(12345, lb.getCell(row, col));
        }
    }

    lb.setCell(1, 1, 0);
    BOOST_CHECK_EQUAL(0, lb.getCell(1, 1));
}

BOOST_AUTO_TEST_CASE( LotteryBoard_mark_isMarked )
{
    LotteryBoard lb(10);

    for(int row=1; row<=10; row++)
    {
        for(int col=1; col<=10; col++)
        {
            lb.mark(row, col);
            BOOST_CHECK_EQUAL(true, lb.isMarked(row, col));
        }
    }
}

BOOST_AUTO_TEST_CASE( LotteryBoard_markall1 )
{
    LotteryBoard lb(10);

    lb.setCell(1, 1, 12345);

    lb.markAll(12345);

    BOOST_CHECK_EQUAL(true, lb.isMarked(1, 1));
}

BOOST_AUTO_TEST_CASE( LotteryBoard_markall2 )
{
    LotteryBoard lb(10);

    lb.setCell(1, 1, 12345);
    lb.setCell(2, 2, 12345);
    lb.setCell(3, 3, 12345);

    lb.markAll(12345);

    BOOST_CHECK_EQUAL(true, lb.isMarked(1, 1));
    BOOST_CHECK_EQUAL(true, lb.isMarked(2, 2));
    BOOST_CHECK_EQUAL(true, lb.isMarked(3, 3));
}

BOOST_AUTO_TEST_CASE( LotteryBoard_win_horizontal )
{
    LotteryBoard lb(3);

    lb.setCell(1, 1, 12345);
    lb.setCell(1, 2, 12345);
    lb.setCell(1, 3, 12345);

    lb.markAll(12345);

    BOOST_CHECK_EQUAL(true, lb.check());
}

BOOST_AUTO_TEST_CASE( LotteryBoard_win_vertical )
{
    LotteryBoard lb(3);

    lb.setCell(1, 1, 12345);
    lb.setCell(2, 1, 12345);
    lb.setCell(3, 1, 12345);

    lb.markAll(12345);

    BOOST_CHECK_EQUAL(true, lb.check());
}

BOOST_AUTO_TEST_CASE( LotteryBoard_diagonal1 )
{
    LotteryBoard lb(3);

    lb.setCell(1, 1, 1);
    lb.setCell(2, 2, 1);
    lb.setCell(3, 3, 1);

    lb.markAll(1);

    BOOST_CHECK_EQUAL(true, lb.checkWithDiagonal());
}

BOOST_AUTO_TEST_CASE( LotteryBoard_diagonal2 )
{
    LotteryBoard lb(3);

    lb.setCell(1, 3, 1);
    lb.setCell(2, 2, 1);
    lb.setCell(3, 1, 1);
    lb.printBoard();

    lb.markAll(1);

    BOOST_CHECK_EQUAL(true, lb.checkWithDiagonal());
}