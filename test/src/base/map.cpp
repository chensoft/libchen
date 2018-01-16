/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/map.hpp"
#include "catch.hpp"

TEST_CASE("BaseMapTest")
{
    using chen::map;

    // exist
    std::map<int, int> m = {{1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 10}};

    CHECK(map::exist(m, 3));
    CHECK_FALSE(map::exist(m, 8));

    // find
    CHECK(8 == map::find(m, 3));
    CHECK(15 == map::find(m, 10, 15));

    // write
    int out = 0;

    map::write(m, 10, out);
    CHECK(0 == out);

    map::write(m, 3, out);
    CHECK(8 == out);

    // keys
    auto keys = map::keys(m);
    std::sort(keys.begin(), keys.end());

    CHECK(std::vector<int>({1, 2, 3, 4, 5}) == keys);

    // values
    auto values = map::values(m);
    std::sort(values.begin(), values.end());

    CHECK(std::vector<int>({6, 7, 8, 9, 10}) == values);

    // flip
    auto flip = map::flip(m);
    std::map<int, int> ref({{6, 1}, {7, 2}, {8, 3}, {9, 4}, {10, 5}});

    CHECK(ref == flip);
}