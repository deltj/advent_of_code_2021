
#define BOOST_TEST_MODULE Day3
#define BOOST_TEST_DYN_LINK

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>
#include <string>

class SubDiagnostics
{
public:
    SubDiagnostics(const int size=5) : 
            report_size(size), 
            num_reports(0), 
            gamma_rate(0), 
            epsilon_rate(0),
            ogr(0),
            csr(0)
    {
        //  Zeroize the sum to start
        for(size_t i=0; i<report_size; i++)
        {
            bit_sum.push_back(0);
        }
    }

    void processReport(const std::string &report)
    {
        if(report.length() != report_size) return;

        int decimal_report = 0;

        //  Consider each bit in the input
        for(size_t i=0; i<report_size; i++)
        {
            const int bit = boost::lexical_cast<int>(report.at(i));

            //  update bit sums
            bit_sum[i] += bit;

            //  update decimal input
            decimal_report <<= 1;
            decimal_report |= bit;
        }

        input.push_back(decimal_report);

        num_reports++;
    }

    int getNumReports() const { return num_reports; }

    void computeGammaAndEpsilonRates()
    {
        gamma_rate = 0;

        for(size_t i=0; i<report_size; i++)
        {
            //  I'm using an average to find the most common bit value
            const int most_common_bit_value = bit_sum[i] > (num_reports / 2) ? 1 : 0;

            gamma_rate <<= 1;
            gamma_rate |= most_common_bit_value;
        }

        const int bitmask = (1 << report_size) - 1;

        epsilon_rate = (~gamma_rate) & bitmask;
    }

    void computeOGRandCSR()
    {
        //  copy input, then filter it down for oxygen generator rating calculation
        std::vector<int> ogr_input = input;

        for(size_t bitnum=1; bitnum<=report_size; bitnum++)
        {
            const int shift_amt = report_size - bitnum;
            int ones = 0;
            int zeroes = 0;

            //  Find the most common value for this bit
            for(std::vector<int>::const_iterator it=ogr_input.begin(); it!=ogr_input.end(); ++it)
            {
                const int bit = (*it >> shift_amt) & 1;

                if(bit == 1) ones++;
                if(bit == 0) zeroes++;
            }
            const int mcv = ones >= zeroes ? 1 : 0;
            //std::cout << "bit " << bitnum << " mcv: " << mcv << std::endl;

            //  For OGR, remove numbers that do not have the most common value for this bit
            auto new_ogr_end = std::remove_if(ogr_input.begin(), ogr_input.end(),
                [shift_amt, mcv](int num) { return ((num >> shift_amt) & 1) != mcv; });
            ogr_input.erase(new_ogr_end, ogr_input.end());

            //std::cout << "Filtered OGR input size: " << ogr_input.size() << std::endl;

            if(ogr_input.size() == 1)
            {
                //std::cout << "OGR value: " << ogr_input[0] << std::endl;
                ogr = ogr_input[0];
                break;
            }
        }

        //  copy input, then filter it down for CO2 scrubber rating calculation
        std::vector<int> csr_input = input;

        for(size_t bitnum=1; bitnum<=report_size; bitnum++)
        {
            const int shift_amt = report_size - bitnum;
            int ones = 0;
            int zeroes = 0;

            //  Find the most common value for this bit
            for(std::vector<int>::const_iterator it=csr_input.begin(); it!=csr_input.end(); ++it)
            {
                const int bit = (*it >> shift_amt) & 1;

                if(bit == 1) ones++;
                if(bit == 0) zeroes++;
            }
            const int lcv = zeroes <= ones ? 0 : 1;
            //std::cout << "bit " << bitnum << " lcv: " << lcv << std::endl;

            //  For CSR, remove numbers that do not have the least common value for this bit
            auto new_csr_end = std::remove_if(csr_input.begin(), csr_input.end(),
                [shift_amt, lcv](int num) { return ((num >> shift_amt) & 1) != lcv; });
            csr_input.erase(new_csr_end, csr_input.end());

            //std::cout << "Filtered CSR input size: " << csr_input.size() << std::endl;

            if(csr_input.size() == 1)
            {
                //std::cout << "CSR value: " << csr_input[0] << std::endl;
                csr = csr_input[0];
                break;
            }
        }
    }

    int getGammaRate() const { return gamma_rate; }
    int getEpsilonRate() const { return epsilon_rate; }
    int getPowerConsumption() const { return gamma_rate * epsilon_rate; }

    int getOxygenGeneratorRating() const { return ogr; }
    int getCO2ScrubberRating() const { return csr; }
    int getLifeSupportRating() const { return ogr * csr; }

private:
    std::vector<int> input;

    //  I'm using a vector to store the sum of all bits by index
    //  (so bit_sum[i] contains the sum of the ith bits from each number)
    std::vector<int> bit_sum;

    size_t report_size;
    int num_reports;
    int gamma_rate;
    int epsilon_rate;

    int ogr;
    int csr;
};

BOOST_AUTO_TEST_CASE( day3_part1_example )
{
    SubDiagnostics d(5);

    //  Test with example input
    std::ifstream ifs("../day3_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        d.processReport(line);
    }

    d.computeGammaAndEpsilonRates();

    BOOST_CHECK_EQUAL(12, d.getNumReports());
    BOOST_CHECK_EQUAL(22, d.getGammaRate());
    BOOST_CHECK_EQUAL(9, d.getEpsilonRate());
    BOOST_CHECK_EQUAL(198, d.getPowerConsumption());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day3_part1_actual )
{
    SubDiagnostics d(12);

    //  Test with real input
    std::ifstream ifs("../day3_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        d.processReport(line);
    }

    d.computeGammaAndEpsilonRates();

    BOOST_CHECK_EQUAL(3259, d.getGammaRate());
    BOOST_CHECK_EQUAL(836, d.getEpsilonRate());
    BOOST_CHECK_EQUAL(2724524, d.getPowerConsumption());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day3_part2_example )
{
    SubDiagnostics d(5);

    //  Test with example input
    std::ifstream ifs("../day3_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        d.processReport(line);
    }

    d.computeOGRandCSR();

    BOOST_CHECK_EQUAL(23, d.getOxygenGeneratorRating());
    BOOST_CHECK_EQUAL(10, d.getCO2ScrubberRating());
    BOOST_CHECK_EQUAL(230, d.getLifeSupportRating());

    ifs.close();
}

BOOST_AUTO_TEST_CASE( day3_part2_actual )
{
    SubDiagnostics d(12);

    //  Test with real input
    std::ifstream ifs("../day3_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    std::string line;

    //  Read the input file one line at a time
    while(std::getline(ifs, line))
    {
        d.processReport(line);
    }

    d.computeOGRandCSR();

    BOOST_CHECK_EQUAL(4023, d.getOxygenGeneratorRating());
    BOOST_CHECK_EQUAL(690, d.getCO2ScrubberRating());
    BOOST_CHECK_EQUAL(2775870, d.getLifeSupportRating());

    ifs.close();
}