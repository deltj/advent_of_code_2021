
#define BOOST_TEST_MODULE Day1
#define BOOST_TEST_DYN_LINK

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <list>
#include <numeric>
#include <string>

class Sonar
{
public:
    Sonar() : inc(0) {}

    void addMeasurement(const int &measurement)
    {
        static int previous_measurement = 0;

        if(previous_measurement != 0 && measurement > previous_measurement)
        {
            inc++;
        }

        previous_measurement = measurement;
    }

    void addMeasurementWindowed(const int &measurement)
    {
        static std::list<int> window;
        static int previous_window_sum = 0;

        window.push_front(measurement);

        if(window.size() != 3)
        {
            return;
        }

        const int window_sum = std::accumulate(window.begin(), window.end(), 0);

        window.pop_back();

        if(previous_window_sum != 0 && window_sum > previous_window_sum)
        {
            inc++;
        }

        previous_window_sum = window_sum;
    }

    int getIncreasingMeasurementCount() const { return inc; }

private:
    int inc;
};

BOOST_AUTO_TEST_CASE( day1_part1_example )
{
    Sonar s;

    //  Test with example input
    std::ifstream ifs("../day1_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        s.addMeasurement(measurement);
    }

    BOOST_CHECK_EQUAL(7, s.getIncreasingMeasurementCount());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day1_part1_actual )
{
    Sonar s;

    //  Test with real input
    std::ifstream ifs("../day1_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        s.addMeasurement(measurement);
    }

    BOOST_CHECK_EQUAL(1466, s.getIncreasingMeasurementCount());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day1_part2_example )
{
    Sonar s;

    //  Test with example input
    std::ifstream ifs("../day1_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        s.addMeasurementWindowed(measurement);
    }

    BOOST_CHECK_EQUAL(5, s.getIncreasingMeasurementCount());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day1_part2_actual )
{
    Sonar s;

    //  Test with real input
    std::ifstream ifs("../day1_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    int measurement = 0;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        measurement = boost::lexical_cast<int>(line);
        s.addMeasurementWindowed(measurement);
    }

    BOOST_CHECK_EQUAL(1491, s.getIncreasingMeasurementCount());

    ifs.close();
}