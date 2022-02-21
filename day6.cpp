
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
            initial_fish.push_back(0);
    }

    void readInitialState(const std::string &stateString)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, stateString, boost::is_any_of(","));

        //double p0 = 0;
        for(std::vector<std::string>::const_iterator it=tokens.begin(); it!=tokens.end(); ++it)
        {
            //p0++;

            int age = boost::lexical_cast<int>(*it);
            initial_fish[age] += 1;
        }
        fish = initial_fish;

        //  p(t) = p(0) * e^kt
        //  26 = p(0) * e^18k
        //  26 / p(0) = e^18k
        //  ln(26 / p(0)) = 18k
        //  ln(26 / p(0)) / 18 = k
        //double k1 = log(26.0 / p0) / 18.0;
        //double k2 = log(5934.0 / p0) / 80.0;
        //  This method doesn't work; We end up with two different growth rates and no solution
    }

    uint64_t calc(const double t)
    {
        fish.clear();
        fish = initial_fish;

        for(int i=0; i<t; i++)
        {
            //  Shift the number of fish at each age down by a day
            uint64_t tmp = fish[0];
            for(int age=0; age<9; age++)
            {
                fish[age] = fish[age + 1];
            }
            fish[6] += tmp;
            fish[8] += tmp;
        }

        uint64_t total_fish = 0;
        for(int i=0; i<9; i++)
        {
            total_fish += fish[i];
        }

        return total_fish;
        //return p0 * exp(k * t);
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
    //lfrm.printFish();
    //lfrm.calc(2);
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
