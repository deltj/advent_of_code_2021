
#define BOOST_TEST_MODULE Day5
#define BOOST_TEST_DYN_LINK

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

#include <fstream>

double determinant(const int mat[2][2])
{
    return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);
}

class Coord2d
{
public:
    Coord2d(const int &_x, const int &_y) : x(_x), y(_y) {}

    bool operator==(const Coord2d &coord)
    {
        return (x == coord.x) && (y == coord.y);
    }

    bool operator!=(const Coord2d &coord)
    {
        return !(*this == coord);
    }

    int x;
    int y;
};

class Line2d
{
public:
    Line2d(const Coord2d &_p1, const Coord2d &_p2) : p1(_p1), p2(_p2) {}

    bool operator==(const Line2d &line)
    {
        return (p1 == line.p1) && (p2 == line.p2);
    }

    bool operator!=(const Line2d &line)
    {
        return !(*this == line);
    }

    bool isHorizontal() const { return (p1.y == p2.y); }
    bool isVertical() const { return (p1.x == p2.x); }

    double getSlope() const { return (p2.y - p1.y) / (p2.x - p1.x); }

    double getB() const
    {
        //  y=mx+b
        //  y-mx=b

        const double m = getSlope();
        return p1.y - m * p1.x;
    }

    //  I didn't end up using this function but I left it here because it's neat
    bool intersects(const Line2d &line)
    {
        //  See: https://en.wikipedia.org/wiki/Intersection_(Euclidean_geometry)#Two_line_segments

        //  We have two line segments defined by their endopoints: (x1,y1),(x2,y2) and (x3,y3),(x4,y4)
        //  I've copied these to stack variables to aid readability
        const int x1 = p1.x;
        const int y1 = p1.y;
        const int x2 = p2.x;
        const int y2 = p2.y;
        const int x3 = line.p1.x;
        const int y3 = line.p1.y;
        const int x4 = line.p2.x;
        const int y4 = line.p2.y;

        //  Solve the linear system below to find s and t using Cramer's Rule
        //  See: https://en.wikipedia.org/wiki/Cramer%27s_rule
        //  s(x2 - x1) - t(x4 - x3) = x3 - x1
        //  s(y2 - y1) - t(y4 - y3) = y3 - y1
        const int  m[2][2] = {{x2 - x1, -(x4 - x3)},
                              {y2 - y1, -(y4 - y3)}};
        const int m1[2][2] = {{x3 - x1, -(x4 - x3)},
                              {y3 - y1, -(y4 - y3)}};
        const int m2[2][2] = {{x2 - x1,   x3 - x1},
                              {y2 - y1,   y3 - y1}};
        const double D  = determinant(m);
        const double D1 = determinant(m1);
        const double D2 = determinant(m2);

        std::cout << "D:  " << D << std::endl;
        std::cout << "D1: " << D1 << std::endl;
        std::cout << "D2: " << D2 << std::endl;

        if(D == 0) return false;

        const double s = D1 / D;
        const double t = D2 / D;

        std::cout << "s: " << s << std::endl;
        std::cout << "t: " << t << std::endl;

        //  Find the intersection point
        const double x = x1 + s * (x2 - x1);
        const double y = y1 + s * (y2 - y1);

        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;

        return (s >= 0) && (t <= 1);
    }

    Coord2d p1;
    Coord2d p2;
};

class HydrothermalVentTracker
{
public:
    HydrothermalVentTracker(const int _size=5) : size(_size), ventmap(nullptr)
    {
        ventmap = new int[size * size];
        memset(ventmap, 0, sizeof(int) * size * size);
    }

    ~HydrothermalVentTracker()
    {
        if(ventmap)
        {
            delete[] ventmap;
        }
    }

    int getVentCount(const int x, const int y)
    {
        return ventmap[x * size + y];
    }

    void updateMap(const Line2d &line)
    {
        //  Handle vertical lines as an edge case
        if(line.isVertical())
        {
            const int x = line.p1.x;
            const Coord2d p = line.p1.y < line.p2.y ? line.p1 : line.p2;
            const Coord2d q = line.p1.y < line.p2.y ? line.p2 : line.p1;
            for(int y=p.y; y<=q.y; y++)
            {
                ventmap[x * size + y] += 1;
                //std::cout << "(" << x << "," << y << ")" << std::endl;
            }
        }
        else
        {
            //  y=mx+b
            const double m = line.getSlope();
            const double b = line.getB();
            const Coord2d p = line.p1.x < line.p2.x ? line.p1 : line.p2;
            const Coord2d q = line.p1.x < line.p2.x ? line.p2 : line.p1;
            for(int x=p.x; x<=q.x; x++)
            {
                int y = m * x + b;
                ventmap[x * size + y] += 1;
                //std::cout << "(" << x << "," << y << ")" << std::endl;
            }
        }
    }

    void updateLinesHV()
    {
        for(std::vector<Line2d>::const_iterator it=hydrothermalVentLines.begin(); it!=hydrothermalVentLines.end(); ++it)
        {
            if(it->isHorizontal() || it->isVertical())
            {
                updateMap(*it);
            }
        }
    }

    void updateLines()
    {
        for(std::vector<Line2d>::const_iterator it=hydrothermalVentLines.begin(); it!=hydrothermalVentLines.end(); ++it)
        {
            updateMap(*it);
        }
    }

    void readVentMap(std::ifstream &ifs)
    {
        std::string line;
        std::vector<std::string> tokens;

        while(std::getline(ifs, line))
        {
            tokens.clear();
            boost::split(tokens, line, boost::is_any_of(", "));

            if(tokens.size() != 5) continue;

            const int x1 = boost::lexical_cast<int>(tokens[0]);
            const int y1 = boost::lexical_cast<int>(tokens[1]);
            const int x2 = boost::lexical_cast<int>(tokens[3]);
            const int y2 = boost::lexical_cast<int>(tokens[4]);

            const Coord2d p1(x1, y1);
            const Coord2d p2(x2, y2);

            const Line2d line(p1, p2);

            hydrothermalVentLines.push_back(line);
        }
    }

    size_t getNumVentLines() const { return hydrothermalVentLines.size(); }

private:
    std::vector<Line2d> hydrothermalVentLines;

    int size;
    int *ventmap;
};

BOOST_AUTO_TEST_CASE( coord_equality )
{
    Coord2d p1(1, 2);
    Coord2d p2(1, 2);
    BOOST_CHECK(p1 == p2);

    Coord2d p3(-1, -1);
    BOOST_CHECK(p1 != p3);
}

BOOST_AUTO_TEST_CASE( line_equality )
{
    Line2d line1(Coord2d(0, 0), Coord2d(5, 5));
    Line2d line2(Coord2d(0, 0), Coord2d(5, 5));
    BOOST_CHECK(line1 == line2);

    Line2d line3(Coord2d(0, 0), Coord2d(6, 6));
    BOOST_CHECK(line1 != line3);
}

BOOST_AUTO_TEST_CASE( line_ishorizontal )
{
    Line2d line(Coord2d(0, 0), Coord2d(5, 0));
    BOOST_CHECK(line.isHorizontal());
}

BOOST_AUTO_TEST_CASE( line_isvertical )
{
    Line2d line(Coord2d(2, 1), Coord2d(2, 2));
    BOOST_CHECK(line.isVertical());
}

BOOST_AUTO_TEST_CASE( line_slope )
{
    Line2d line(Coord2d(0, 0), Coord2d(5, 5));
    BOOST_CHECK_EQUAL(1, line.getSlope());
}

BOOST_AUTO_TEST_CASE( intersect )
{
    Line2d line1(Coord2d(1, 1), Coord2d(3, 2));
    Line2d line2(Coord2d(1, 4), Coord2d(2, -1));

    BOOST_CHECK(line1.intersects(line2));
}

BOOST_AUTO_TEST_CASE( updateMap )
{
    HydrothermalVentTracker hvt(5);
    Line2d line1(Coord2d(0,0), Coord2d(4,4));
    Line2d line2(Coord2d(0,0), Coord2d(0,4));

    BOOST_CHECK_EQUAL(0, hvt.getVentCount(0, 0));

    hvt.updateMap(line1);

    BOOST_CHECK_EQUAL(1, hvt.getVentCount(0, 0));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(1, 1));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(2, 2));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(3, 3));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(4, 4));

    hvt.updateMap(line2);

    BOOST_CHECK_EQUAL(2, hvt.getVentCount(0, 0));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(0, 1));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(0, 2));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(0, 3));
    BOOST_CHECK_EQUAL(1, hvt.getVentCount(0, 4));
}

BOOST_AUTO_TEST_CASE( day5_part1_example )
{
    const int map_size = 10;
    HydrothermalVentTracker hvt(map_size);

    //  Test with example input
    std::ifstream ifs("../day5_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    hvt.readVentMap(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(10, hvt.getNumVentLines());

    hvt.updateLinesHV();

    int number_of_points_where_at_least_two_lines_overlap = 0;
    for(int y=0; y<map_size; y++)
    {
        for(int x=0; x<map_size; x++)
        {
            std::cout << hvt.getVentCount(x, y) << " ";
            if(hvt.getVentCount(x, y) > 1)
            {
                number_of_points_where_at_least_two_lines_overlap++;
            }
        }
        std::cout << std::endl;
    }
    BOOST_CHECK_EQUAL(5, number_of_points_where_at_least_two_lines_overlap);
}

BOOST_AUTO_TEST_CASE( day5_part1_actual )
{
    const int map_size = 1000;
    HydrothermalVentTracker hvt(map_size);

    //  Test with real input
    std::ifstream ifs("../day5_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    hvt.readVentMap(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(500, hvt.getNumVentLines());

    hvt.updateLinesHV();

    int number_of_points_where_at_least_two_lines_overlap = 0;
    for(int y=0; y<map_size; y++)
    {
        for(int x=0; x<map_size; x++)
        {
            if(hvt.getVentCount(x, y) > 1)
            {
                number_of_points_where_at_least_two_lines_overlap++;
            }
        }
    }
    BOOST_CHECK_EQUAL(4728, number_of_points_where_at_least_two_lines_overlap);
}

BOOST_AUTO_TEST_CASE( day5_part2_example )
{
    const int map_size = 10;
    HydrothermalVentTracker hvt(map_size);

    //  Test with example input
    std::ifstream ifs("../day5_example_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    hvt.readVentMap(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(10, hvt.getNumVentLines());

    hvt.updateLines();

    int number_of_points_where_at_least_two_lines_overlap = 0;
    for(int y=0; y<map_size; y++)
    {
        for(int x=0; x<map_size; x++)
        {
            std::cout << hvt.getVentCount(x, y) << " ";
            if(hvt.getVentCount(x, y) > 1)
            {
                number_of_points_where_at_least_two_lines_overlap++;
            }
        }
        std::cout << std::endl;
    }
    BOOST_CHECK_EQUAL(12, number_of_points_where_at_least_two_lines_overlap);
}

BOOST_AUTO_TEST_CASE( day5_part2_actual )
{
    const int map_size = 1000;
    HydrothermalVentTracker hvt(map_size);

    //  Test with example input
    std::ifstream ifs("../day5_input", std::ifstream::in);
    BOOST_REQUIRE(ifs.is_open());

    hvt.readVentMap(ifs);

    ifs.close();

    BOOST_CHECK_EQUAL(500, hvt.getNumVentLines());

    hvt.updateLines();

    int number_of_points_where_at_least_two_lines_overlap = 0;
    for(int y=0; y<map_size; y++)
    {
        for(int x=0; x<map_size; x++)
        {
            if(hvt.getVentCount(x, y) > 1)
            {
                number_of_points_where_at_least_two_lines_overlap++;
            }
        }
    }
    BOOST_CHECK_EQUAL(17717, number_of_points_where_at_least_two_lines_overlap);
}