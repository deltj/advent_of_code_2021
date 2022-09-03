
#define BOOST_TEST_MODULE Day8
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <vector>

class SevenSegmentData
{
public:

    /**
     * @brief Read lines of input from the specified stream.
     * 
     * @details Lines are expected to look like:
     * acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf
     * 
     * The symbols before the pipe are observed signal patterns, and the symbols after the
     * pipe are a four digit output value.
     * 
     * @param is The input stream to read from
     */
    void readInput(std::istream &is)
    {
        signalPatterns.clear();
        output.clear();
        
        std::string line;
        std::vector<std::string> tokens1;
        std::vector<std::string> tokens2;
        std::vector<std::string> tokens3;

        while(std::getline(is, line))
        {
            tokens1.clear();
            boost::split(tokens1, line, boost::is_any_of("|"));

            if(tokens1.size() == 2)
            {
                boost::algorithm::trim(tokens1[0]);
                boost::algorithm::trim(tokens1[1]);

                tokens2.clear();
                tokens3.clear();

                boost::split(tokens2, tokens1[0], boost::is_any_of(" "));
                boost::split(tokens3, tokens1[1], boost::is_any_of(" "));

                if(tokens2.size() >= 10 && tokens3.size() >= 4)
                {
                    for(size_t i=0; i<tokens2.size(); i++)
                    {
                        std::sort(tokens2[i].begin(), tokens2[i].end());
                    }

                    for(size_t i=0; i<tokens3.size(); i++)
                    {
                        std::sort(tokens3[i].begin(), tokens3[i].end());
                    }

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

    bool mapIsComplete(std::map<int, std::string> dmap)
    {
        std::cout << "mapsize: " << dmap.size() << std::endl;
        for(int i=0; i<10; i++)
        {
            if(dmap.find(i) == dmap.end())
            {
                return false;
            }
        }
        return true;
    }

    std::map<std::string, int> buildPatternMap(std::vector<std::string> patterns)
    {
        std::map<std::string, int> patternToDigit;
        std::map<int, std::string> digitToPattern;

        //  find the digits with unique lengths
        for(auto p : patterns)
        {
            switch(p.length())
            {
            case 2: //1
                patternToDigit[p] = 1;
                digitToPattern[1] = p;
                break;

            case 4: //4
                patternToDigit[p] = 4;
                digitToPattern[4] = p;
                break;

            case 3: //7
                patternToDigit[p] = 7;
                digitToPattern[7] = p;
                break;

            case 7: //8
                patternToDigit[p] = 8;
                digitToPattern[8] = p;
                break;

            default:
                break;
            }
        }

        //  Known: 1, 4, 7, 8

        for(auto p : patterns)
        {
            if(p.length() == 5)
            {
                //  3 includes the segments from 1
                if(std::includes(p.begin(), p.end(), digitToPattern[1].begin(), digitToPattern[1].end()))
                {
                    patternToDigit[p] = 3;
                    digitToPattern[3] = p;
                }
            }

            if(p.length() == 6)
            {
                //  9 includes the segments from 4
                if(std::includes(p.begin(), p.end(), digitToPattern[4].begin(), digitToPattern[4].end()))
                {
                    patternToDigit[p] = 9;
                    digitToPattern[9] = p;
                }
            }
        }

        //  Known: 1, 3, 4, 7, 8, 9

        //  Segment d is in 3 and 4 but not 7
        char d;
        for(auto ch : digitToPattern[3])
        {
            if(digitToPattern[4].find(ch) != std::string::npos && digitToPattern[7].find(ch) == std::string::npos)
            {
                d = ch;
            }
        }

        //  Zero is the only digit with 6 segments excluding d
        for(auto p : patterns)
        {
            if((p.length() == 6) && (p.find(d) == std::string::npos))
            {
                patternToDigit[p] = 0;
                digitToPattern[0] = p;
            }
        }

        //  Known: 0, 1, 3, 4, 7, 8, 9

        //  Six is the only 6-segment digit left
        for(auto p : patterns)
        {
            //  If length is six and the pattern is not yet mapped
            if((p.length() == 6) && (patternToDigit.find(p) == patternToDigit.end()))
            {
                patternToDigit[p] = 6;
                digitToPattern[6] = p;
            }
        }

        //  Segment c is in 0 and not 6
        char c;
        for(auto ch : digitToPattern[0])
        {
            if(digitToPattern[6].find(ch) == std::string::npos)
            {
                c = ch;
            }
        }

        //  5 does not use segment c
        for(auto p : patterns)
        {
            if((p.find(c) == std::string::npos) && (patternToDigit.find(p) == patternToDigit.end()))
            {
                patternToDigit[p] = 5;
                digitToPattern[5] = p;
            }
        }

        //  The remaining un-mapped digit must be 2
        for(auto p : patterns)
        {
            if(patternToDigit.find(p) == patternToDigit.end())
            {
                patternToDigit[p] = 2;
                digitToPattern[2] = p;
            }
        }

        return patternToDigit;
    }

    int decode(std::vector<std::string> output, std::map<std::string, int> pmap)
    {
        int val = 0;
        for(auto o : output)
        {
            val = val * 10;
            val += pmap[o];
        }
        return val;
    }

    int getOutputValue(const int &index)
    {
        std::map<std::string, int> pmap = buildPatternMap(signalPatterns[index]);
        return decode(output[index], pmap);
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

BOOST_AUTO_TEST_CASE( day8_part2_small_example )
{
    //  Test with example input
    std::ifstream ifs("../day8_small_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    SevenSegmentData ssd;
    ssd.readInput(ifs);
    ifs.close();

    BOOST_CHECK_EQUAL(1, ssd.numRecords());
    
    std::map<std::string, int> pmap = ssd.buildPatternMap(ssd.signalPatterns[0]);
    BOOST_CHECK_EQUAL(5353, ssd.decode(ssd.output[0], pmap));

    BOOST_CHECK_EQUAL(5353, ssd.getOutputValue(0));
}

BOOST_AUTO_TEST_CASE( day8_part2_debug1 )
{
    const std::string input = "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe";
    std::istringstream is(input);

    SevenSegmentData ssd;
    ssd.readInput(is);

    BOOST_CHECK_EQUAL(1, ssd.numRecords());
    BOOST_CHECK_EQUAL(4548, ssd.getOutputValue(0));

    /*
    std::map<std::string, int> pmap = ssd.buildPatternMap(ssd.signalPatterns[0]);
    for(auto m : pmap)
    {
        std::cout << m.first << " = " << m.second << std::endl;
    }
    */
}

BOOST_AUTO_TEST_CASE( day8_part2_debug2 )
{
    const std::string input = "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef";
    std::istringstream is(input);

    SevenSegmentData ssd;
    ssd.readInput(is);

    BOOST_CHECK_EQUAL(1, ssd.numRecords());
    BOOST_CHECK_EQUAL(1625, ssd.getOutputValue(0));

    /*
    std::map<std::string, int> pmap = ssd.buildPatternMap(ssd.signalPatterns[0]);
    for(auto m : pmap)
    {
        std::cout << m.first << " = " << m.second << std::endl;
    }
    */
}

BOOST_AUTO_TEST_CASE( day8_part2_example )
{
    //  Test with example input
    std::ifstream ifs("../day8_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    SevenSegmentData ssd;
    ssd.readInput(ifs);
    ifs.close();

    BOOST_CHECK_EQUAL(10, ssd.numRecords());
    
    BOOST_CHECK_EQUAL(8394, ssd.getOutputValue(0));
    BOOST_CHECK_EQUAL(9781, ssd.getOutputValue(1));
    BOOST_CHECK_EQUAL(1197, ssd.getOutputValue(2));
    BOOST_CHECK_EQUAL(9361, ssd.getOutputValue(3));
    BOOST_CHECK_EQUAL(4873, ssd.getOutputValue(4));
    BOOST_CHECK_EQUAL(8418, ssd.getOutputValue(5));
    BOOST_CHECK_EQUAL(4548, ssd.getOutputValue(6));
    BOOST_CHECK_EQUAL(1625, ssd.getOutputValue(7));
    BOOST_CHECK_EQUAL(8717, ssd.getOutputValue(8));
    BOOST_CHECK_EQUAL(4315, ssd.getOutputValue(9));

    /*
    std::map<std::string, int> pmap = ssd.buildPatternMap(ssd.signalPatterns[6]);
    for(auto m : pmap)
    {
        std::cout << m.first << " = " << m.second << std::endl;
    }
    */

    //  Sum the output values
    long sum = 0;
    for(size_t i=0; i<ssd.numRecords(); i++)
    {
        sum += ssd.getOutputValue(i);
    }

    BOOST_CHECK_EQUAL(61229, sum);
}

BOOST_AUTO_TEST_CASE( day8_part2_actual )
{
    //  Test with example input
    std::ifstream ifs("../day8_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    SevenSegmentData ssd;
    ssd.readInput(ifs);
    ifs.close();

    BOOST_CHECK_EQUAL(200, ssd.numRecords());

    /*
    std::map<std::string, int> pmap = ssd.buildPatternMap(ssd.signalPatterns[6]);
    for(auto m : pmap)
    {
        std::cout << m.first << " = " << m.second << std::endl;
    }
    */

    //  Sum the output values
    int sum = 0;
    for(size_t i=0; i<ssd.numRecords(); i++)
    {
        //std::cout << ssd.getOutputValue(i) << std::endl;
        sum += ssd.getOutputValue(i);
    }

    BOOST_CHECK_EQUAL(1043697, sum);
}
