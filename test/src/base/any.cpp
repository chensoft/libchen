/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/any.hpp"
#include "catch.hpp"

TEST_CASE("BaseAnyTest")
{
    using chen::any;

    // empty
    any val;

    CHECK(val.empty());

    // non-empty
    val = 15;

    CHECK_FALSE(val.empty());

    // integer
    int a = val;

    CHECK(0 != a);
    CHECK(10 != a);

    CHECK(15 == a);

    // string
    val = std::string("chensoft.com");
    std::string s = val;

    CHECK(std::string("chensoft") != s);
    CHECK(std::string("chensoft.com") == s);

    // assign
    any o1(std::move(val));

    CHECK(val.empty());
    CHECK_FALSE(o1.empty());

    any o2;
    o2 = std::move(o1);

    CHECK(o1.empty());
    CHECK_FALSE(o2.empty());

    any o3;

    o3 = static_cast<const any&>(o3);  // self assign
    CHECK(o3.empty());

    o3 = 15;
    o3 = static_cast<const any&>(o1);  // test assign empty object
    CHECK(o3.empty());

    o3 = std::move(o1);  // self rvalue assign
    CHECK(o3.empty());
}