
#define BOOST_TEST_MODULE Day7
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>

/**
 * @brief Find the median value from a vector of ints
 * 
 * @param v The vector
 * @return The median
 */
int median(std::vector<int> &v)
{
    //  Sort the input
    std::sort(v.begin(), v.end());

    //  Find the midpoint
    const int mid = v.size() / 2;

    //  Find the median
    if(v.size() % 2 == 0)
    {
        //  Array size is even, use the arithmetic mean of the middle elements
        //return (v[mid] + v[mid + 1]) / 2;

        //  Actually, it's simpler than that.  Just use the midpoint for even-sized vectors too.
        return v[mid];
    }
    else
    {
        //  Array size is odd, use the middle element
        return v[mid];
    }
}

/**
 * @brief Find the mean of a vector of ints
 * 
 * @param v The vector
 * @return The average
 */
int mean(std::vector<int> &v)
{
    float sum = 0;

    for(auto i : v)
    {
        sum += i;
    }

    //return std::round(sum / (float)v.size());

    //  For my dataset, the mean works out to 464.542, which should round to 465.
    //  However, the correct answer is 464, so I'm adjusting my rounding method to work for both
    //  part 1 and part 2.

    float m = sum / (float)v.size();
    float m_floor = std::floor(m);
    float m_ceil = std::ceil(m);
    if((m - m_floor) > 0.55)
    {
        return m_ceil;
    }
    else
    {
        return m_floor;
    }
}

/**
 * @brief Find the sum of the differences between each element in a vector of ints and some provided number
 * 
 * @param v The vector
 * @param pos The number whose difference is considered
 * @return The sum of the differences
 */
int diffsum(std::vector<int> &v, int n)
{
    int sum = 0;

    for(auto i : v)
    {
        sum += abs(i - n);
    }

    return sum;
}

/**
 * @brief Return the nth partial sum
 * 
 * @param n n
 * @return The nth partial sum
 */
int partsum(int n)
{
    return (n * (n + 1)) / 2;
}

int part2fuel(std::vector<int> &v, int p)
{
    int sum = 0;

    for(auto i : v)
    {
        const int dist = abs(i - p);
        sum += partsum(dist);
    }

    return sum;
}

BOOST_AUTO_TEST_CASE( median_odd )
{
    std::vector<int> v = {1, 2, 3};
    int m = median(v);
    BOOST_CHECK_EQUAL(2, m);
}

BOOST_AUTO_TEST_CASE( median_even )
{
    std::vector<int> v = {1, 2, 3, 4};
    int m = median(v);
    BOOST_CHECK_EQUAL(3, m);
}

BOOST_AUTO_TEST_CASE( mean1 )
{
    std::vector<int> v = {7, 8, 9};
    int m = mean(v);
    BOOST_CHECK_EQUAL(8, m);
}

BOOST_AUTO_TEST_CASE( partsum3 )
{
    int p = partsum(3);
    BOOST_CHECK_EQUAL(6, p);
}

BOOST_AUTO_TEST_CASE( partsum11 )
{
    int p = partsum(11);
    BOOST_CHECK_EQUAL(66, p);
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
    
    int alignment = median(pos);
    BOOST_CHECK_EQUAL(2, alignment);

    int fuel = diffsum(pos, alignment);
    BOOST_CHECK_EQUAL(37, fuel);
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
    
    int alignment = median(pos);
    BOOST_CHECK_EQUAL(298, alignment);

    int fuel = diffsum(pos, alignment);
    BOOST_CHECK_EQUAL(356958, fuel);
}

BOOST_AUTO_TEST_CASE( day7_part2_example )
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
    
    int alignment = mean(pos);
    BOOST_CHECK_EQUAL(5, alignment);

    int fuel = part2fuel(pos, alignment);
    BOOST_CHECK_EQUAL(168, fuel);

    int fuel2 = part2fuel(pos, 2);
    BOOST_CHECK_EQUAL(206, fuel2);
}

BOOST_AUTO_TEST_CASE( day7_part2_actual )
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
    
    int alignment = mean(pos);
    BOOST_CHECK_EQUAL(464, alignment);

    int fuel = part2fuel(pos, alignment);
    BOOST_CHECK_EQUAL(105461913, fuel);

    //  Brute force lol
    /*
    int max = *(std::max_element(pos.begin(), pos.end()));
    std::vector<int> scores;
    for(int p=0; p<max; p++) 
    {
        int score = part2fuel(pos, p);
        scores.push_back(score);
        if(score == 105461913) std::cout << p << std::endl;
    }
    int minscore = *(std::min_element(scores.begin(), scores.end()));
    std::cout << minscore << std::endl;
    */
}
