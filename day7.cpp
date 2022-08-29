
#define BOOST_TEST_MODULE Day6
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <fstream>
#include <vector>

//  If each crab movement costs 1 fuel, then the fuel cost to move crab c to position P:
//    Fc = abs(Pc - P)
//  And the total cost to align all crabs is:
//    F = sum(Pi - P) for i = 0 to n
//  We need to find a position Pa that minimizes this cost

//  The position with the minimum movement cost is the median of all crab positions

int mean(std::vector<int> &v)
{
    //  Sort the input
    std::sort(v.begin(), v.end());

    int mid = v.size() / 2;

    //  Find the median
    if(v.size() % 2 == 0)
    {
        //  Array size is even, use the arithmetic mean of the middle elements
        //return (v[mid] + v[mid + 1]) / 2;
        return v[mid];
    }
    else
    {
        //  Array size is odd, use the middle element
        return v[mid];
    }
}

int sumdist(std::vector<int> &v, int pos)
{
    int sum = 0;

    for(auto i : v)
    {
        sum += abs(i - pos);
    }

    return sum;
}

BOOST_AUTO_TEST_CASE( median_odd )
{
    std::vector<int> v = {1, 2, 3};
    int m = mean(v);
    BOOST_CHECK_EQUAL(2, m);
}

BOOST_AUTO_TEST_CASE( median_even )
{
    std::vector<int> v = {1, 2, 3, 4};
    int m = mean(v);
    BOOST_CHECK_EQUAL(3, m);
}

BOOST_AUTO_TEST_CASE( day7_part1_example )
{
    //  Test with example input
    std::ifstream ifs("../day7_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    std::getline(ifs, line);
    ifs.close();

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(", "));

    std::vector<int> pos;
    for(auto t : tokens)
    {
        const int p = boost::lexical_cast<int>(t);
        pos.push_back(p);
    }
    
    int m = mean(pos);
    BOOST_CHECK_EQUAL(2, m);

    int s = sumdist(pos, m);
    BOOST_CHECK_EQUAL(37, s);
}

BOOST_AUTO_TEST_CASE( day7_part1_actual )
{
    //  Test with example input
    std::ifstream ifs("../day7_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    std::getline(ifs, line);
    ifs.close();

    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(", "));

    std::vector<int> pos;
    for(auto t : tokens)
    {
        const int p = boost::lexical_cast<int>(t);
        pos.push_back(p);
    }
    
    int m = mean(pos);
    BOOST_CHECK_EQUAL(298, m);

    int f = sumdist(pos, m);
    BOOST_CHECK_EQUAL(356958, f);
}
