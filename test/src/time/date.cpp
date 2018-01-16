/**
 * Created by Jian Chen
 * @since  2017.06.07
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/time/date.hpp"
#include "catch.hpp"

TEST_CASE("TimeDateTest")
{
    chen::date d;

    CHECK(d.microsecond >= 0);
    CHECK(d.microsecond <= 999999);
    CHECK(d.second >= 0);
    CHECK(d.second <= 59);
    CHECK(d.minute >= 0);
    CHECK(d.minute <= 59);
    CHECK(d.hour >= 0);
    CHECK(d.hour <= 23);
    CHECK(d.day >= 1);
    CHECK(d.day <= 31);
    CHECK(d.wday >= 0);
    CHECK(d.wday <= 6);
    CHECK(d.month >= 1);
    CHECK(d.month <= 12);
    CHECK(d.year >= 2017);
    CHECK(d.zone >= 0);
}