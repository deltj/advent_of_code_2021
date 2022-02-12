
#define BOOST_TEST_MODULE Day2
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

typedef std::vector<std::string> splitvec;

class Submarine
{
public:
    Submarine() : aim(0), hpos(0), depth(0) {}

    void processCommand1(const std::string &cmd)
    {
        splitvec tokens;
        boost::split(tokens, cmd, boost::is_any_of(" "));

        if(tokens.size() != 2) return;

        const std::string direction = tokens[0];
        const int magnitude = boost::lexical_cast<int>(tokens[1]);

        if(direction == "forward")
        {
            hpos += magnitude;
        }
        else if(direction == "up")
        {
            depth -= magnitude;
        }
        else if(direction == "down")
        {
            depth += magnitude;
        }
    }

    void processCommand2(const std::string &cmd)
    {
        splitvec tokens;
        boost::split(tokens, cmd, boost::is_any_of(" "));

        if(tokens.size() != 2) return;

        const std::string direction = tokens[0];
        const int magnitude = boost::lexical_cast<int>(tokens[1]);

        if(direction == "forward")
        {
            hpos += magnitude;
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
    }

    int getHorizontalPos() const { return hpos; }
    int getDepth() const { return depth; }

private:
    int aim;
    int hpos;
    int depth;
};

BOOST_AUTO_TEST_CASE( day2_part1_example )
{
    Submarine s;

    //  Test with example input
    std::ifstream ifs("../day2_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        s.processCommand1(line);
    }

    BOOST_CHECK_EQUAL(15, s.getHorizontalPos());
    BOOST_CHECK_EQUAL(10, s.getDepth());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day2_part1_actual )
{
    Submarine s;

    //  Test with real input
    std::ifstream ifs("../day2_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        s.processCommand1(line);
    }

    BOOST_CHECK_EQUAL(1868, s.getHorizontalPos());
    BOOST_CHECK_EQUAL(1090, s.getDepth());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day2_part2_example )
{
    Submarine s;

    //  Test with example input
    std::ifstream ifs("../day2_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        s.processCommand2(line);
    }

    BOOST_CHECK_EQUAL(15, s.getHorizontalPos());
    BOOST_CHECK_EQUAL(60, s.getDepth());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day2_part2_actual )
{
    Submarine s;

    //  Test with real input
    std::ifstream ifs("../day2_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        s.processCommand2(line);
    }

    BOOST_CHECK_EQUAL(1868, s.getHorizontalPos());
    BOOST_CHECK_EQUAL(1078987, s.getDepth());

    ifs.close();
}