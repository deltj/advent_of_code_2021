
#define BOOST_TEST_MODULE Day8
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>

class SevenSegmentData
{
public:
    void readInput(std::ifstream &ifs)
    {
        signalPatterns.clear();
        output.clear();
        
        std::string line;
        std::vector<std::string> tokens1;
        std::vector<std::string> tokens2;
        std::vector<std::string> tokens3;

        while(std::getline(ifs, line))
        {
            tokens1.clear();
            boost::split(tokens1, line, boost::is_any_of("|"));

            if(tokens1.size() == 2)
            {
                tokens2.clear();
                tokens3.clear();

                boost::split(tokens2, tokens1[0], boost::is_any_of(" "));
                boost::split(tokens3, tokens1[1], boost::is_any_of(" "));

                if(tokens2.size() >= 10 && tokens3.size() >= 4)
                {
                    signalPatterns.push_back(tokens2);
                    output.push_back(tokens3);
                }
            }
        }
    }

    int countUniqueDigits()
    {
        int unique_digits = 0;

        for(auto o : output)
        {
            for(auto disp : o)
            {
                const int num_segments_illuminated = disp.length();

                switch(num_segments_illuminated)
                {
                case 2: //1
                case 4: //4
                case 3: //7
                case 7: //8
                    unique_digits++;
                    break;
                default:
                    break;
                }
            }
        }

        return unique_digits;
    }

    size_t numRecords() { return signalPatterns.size(); }

    std::vector<std::vector<std::string> > signalPatterns;
    std::vector<std::vector<std::string> > output;
};

BOOST_AUTO_TEST_CASE( day8_part1_example )
{
    //  Test with example input
    std::ifstream ifs("../day8_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    SevenSegmentData ssd;
    ssd.readInput(ifs);
    ifs.close();

    BOOST_CHECK_EQUAL(10, ssd.numRecords());
    BOOST_CHECK_EQUAL(26, ssd.countUniqueDigits());
}

BOOST_AUTO_TEST_CASE( day8_part1_actual )
{
    //  Test with example input
    std::ifstream ifs("../day8_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    SevenSegmentData ssd;
    ssd.readInput(ifs);
    ifs.close();

    BOOST_CHECK_EQUAL(200, ssd.numRecords());
    BOOST_CHECK_EQUAL(416, ssd.countUniqueDigits());
}
