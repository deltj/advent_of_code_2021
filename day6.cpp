
#define BOOST_TEST_MODULE Day6
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <fstream>
#include <vector>

//  I *really* wanted this to work as an exponential growth/decay problem, but it doesn't :/

class LanternFishReproductiveModel
{
public:
    LanternFishReproductiveModel()
    {
        //  Initialize the fish vector
        for(int i=0; i<9; i++)
        {
            initial_fish.push_back(0);
        }
    }

    void readInitialState(const std::string &stateString)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, stateString, boost::is_any_of(","));

        for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
        {
            int age = boost::lexical_cast<int>(*it);
            initial_fish[age] += 1;
        }
    }

    uint64_t calc(const int t)
    {
        fish.clear();
        fish = initial_fish;

        for(int i=0; i<t; i++)
        {
            //  Shift the number of fish at each age down by a day
            uint64_t tmp = fish[0];
            for(int age=0; age<8; age++)
            {
                fish[age] = fish[age + 1];
            }

            //  Fish with zero days left reset to 6 days, and create a new 8 day fish
            fish[6] += tmp;
            fish[8] = tmp;
        }

        uint64_t total_fish = 0;
        for(int i=0; i<9; i++)
        {
            total_fish += fish[i];
        }

        return total_fish;
    }

    void printFish() const
    {
        for(int i=0; i<9; i++)
        {
            std::cout << "fish[" << i << "]: " << fish[i] << std::endl;
        }
    }

private:
    std::vector<uint64_t> initial_fish;
    std::vector<uint64_t> fish;
};

BOOST_AUTO_TEST_CASE( day6_part1_example )
{
    //  Test with example input
    std::ifstream ifs("../day6_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    std::getline(ifs, line);

    ifs.close();

    LanternFishReproductiveModel lfrm;
    lfrm.readInitialState(line);
    lfrm.calc(1);
    BOOST_CHECK_EQUAL(5, lfrm.calc(1));
    BOOST_CHECK_EQUAL(6, lfrm.calc(2));
    BOOST_CHECK_EQUAL(7, lfrm.calc(3));
    //lfrm.printFish();

    BOOST_CHECK_EQUAL(26, lfrm.calc(18));
    BOOST_CHECK_EQUAL(5934, lfrm.calc(80));
    BOOST_CHECK_EQUAL(26984457539, lfrm.calc(256));
}

BOOST_AUTO_TEST_CASE( day6_part1_actual )
{
    //  Test with real input
    std::ifstream ifs("../day6_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;
    std::getline(ifs, line);

    ifs.close();

    LanternFishReproductiveModel lfrm;
    lfrm.readInitialState(line);

    BOOST_CHECK_EQUAL(362639, lfrm.calc(80));
    BOOST_CHECK_EQUAL(1639854996917, lfrm.calc(256));
}
