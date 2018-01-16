/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/num.hpp"
#include "catch.hpp"

TEST_CASE("BaseNumTest")
{
    using chen::num;

    // equal
    CHECK(num::equal(0.1, 0.1));
    CHECK(num::equal(0.1f, 0.100000001490116119715002586f));
    CHECK(num::equal(0.1, 0.1000000000000000055511151231257001365183510300715002586));
    CHECK(num::equal(0.1L, 0.1000000000000000000011151231257001365183510300715002586L));

    // swap
    CHECK(15 == num::swap(static_cast<std::int8_t>(15)));
    CHECK(4660 == num::swap(static_cast<std::int16_t>(13330)));
    CHECK(305419896 == num::swap(static_cast<std::int32_t>(2018915346)));
    CHECK(601138196360724480 == num::swap(static_cast<std::int64_t>(201891534600)));
    CHECK(static_cast<std::uint16_t>(4660) == num::swap(static_cast<std::uint16_t>(13330)));
    CHECK(static_cast<std::uint32_t>(305419896) == num::swap(static_cast<std::uint32_t>(2018915346)));
    CHECK(static_cast<std::uint64_t>(601138196360724480) == num::swap(static_cast<std::uint64_t>(201891534600)));

    // str
    CHECK("true" == num::str(true));
    CHECK("false" == num::str(false));
    CHECK("115" == num::str(115));
    CHECK("115" == num::str(115u));
    CHECK("-115" == num::str(-115));

    CHECK("115" == num::str(115.0f));  // because I use %g format
    CHECK("115" == num::str(115.0));
    CHECK("115" == num::str(115.0L));

    // bits
    CHECK(0 == num::bits(0u));
    CHECK(1 == num::bits(0b1));
    CHECK(4 == num::bits(0b1111));
    CHECK(7 == num::bits(0b11111110));
    CHECK(8 == num::bits(0b11111111));
    CHECK(14 == num::bits(0b1111111011111110));
    CHECK(16 == num::bits(0b1111111111111111));
    CHECK(28 == num::bits(0b11111111111111111111111111110000));
    CHECK(31 == num::bits(0b11111111111111111111111111111110));
    CHECK(32 == num::bits(0b11111111111111111111111111111111));

    // random
    auto rand_real = num::random(0.0, 2.0);
    auto rand_int  = num::random(1, 9999);

    CHECK(rand_real >= 0);
    CHECK(rand_real < 2);

    CHECK(rand_int >= 1);
    CHECK(rand_int <= 9999);

    // prime
    CHECK_FALSE(num::prime(0));
    CHECK_FALSE(num::prime(1));
    CHECK(num::prime(2));
    CHECK(num::prime(3));
    CHECK_FALSE(num::prime(4));
    CHECK(num::prime(5));
    CHECK_FALSE(num::prime(6));
    CHECK(num::prime(7));
    CHECK_FALSE(num::prime(8));
    CHECK_FALSE(num::prime(9));
}