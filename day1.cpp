
#define BOOST_TEST_MODULE Day1
#define BOOST_TEST_DYN_LINK

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

/**
 * @brief Count the number of measurements that are larger than the previous measurement
 * 
 * Running time is O(n)
 * 
 * @param measurements A vector of measurements
 * @return The number of increasing measurements
 */
int CountIncreasingMeasurements(const std::vector<int> &measurements)
{
    int inc = 0;
    for(size_t i=1; i<measurements.size(); i++)
    {
        if(measurements[i] > measurements[i - 1])
        {
            inc++;
        }
    }
    return inc;
}

/**
 * @brief Count the number of measurements using a three-measurement sliding window
 * 
 * Running time is O(n)
 * 
 * @param measurements A vector of measurements
 * @return The number of increasing measurements
 */
int CountIncreasingMeasurementsWin(const std::vector<int> &measurements)
{
    int inc = 0;
    for(size_t i=3; i<measurements.size(); i++)
    {
        int prev_win_sum = measurements[i - 3] + measurements[i - 2] + measurements[i - 1];
        int this_win_sum = measurements[i - 2] + measurements[i - 1] + measurements[i];
        if(this_win_sum > prev_win_sum)
        {
            inc++;
        }
    }
    return inc;
}

BOOST_AUTO_TEST_CASE( day1_part1_example )
{
    //  Test with example input
    std::ifstream ifs("../day1_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;
    std::vector<int> measurements;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        measurements.push_back(measurement);
    }
    ifs.close();

    int increasing_measurement_count = CountIncreasingMeasurements(measurements);

    BOOST_CHECK_EQUAL(7, increasing_measurement_count);
}

BOOST_AUTO_TEST_CASE( day1_part1_actual )
{
    //  Test with real input
    std::ifstream ifs("../day1_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;
    std::vector<int> measurements;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        measurements.push_back(measurement);
    }
    ifs.close();

    int increasing_measurement_count = CountIncreasingMeasurements(measurements);
    BOOST_CHECK_EQUAL(1466, increasing_measurement_count);
}

BOOST_AUTO_TEST_CASE( day1_part2_example )
{
    //  Test with example input
    std::ifstream ifs("../day1_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;
    std::vector<int> measurements;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        measurements.push_back(measurement);
    }
    ifs.close();

    int increasing_measurement_count = CountIncreasingMeasurementsWin(measurements);
    BOOST_CHECK_EQUAL(5, increasing_measurement_count);
}

BOOST_AUTO_TEST_CASE( day1_part2_actual )
{
    //  Test with real input
    std::ifstream ifs("../day1_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;
    std::vector<int> measurements;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        measurements.push_back(measurement);
    }
    ifs.close();

    int increasing_measurement_count = CountIncreasingMeasurementsWin(measurements);
    BOOST_CHECK_EQUAL(1491, increasing_measurement_count);    
}
