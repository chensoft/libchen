/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/regex.hpp"
#include "catch.hpp"

TEST_CASE("BaseRegexTest")
{
    using chen::regex;

    std::string pattern(R"(^\d+$)");
    CHECK(regex::match("12345", pattern));

    pattern = R"(soft\.com)";
    CHECK("chenjian" == regex::replace("chensoft.com", pattern, "jian"));

    CHECK("0.0.0.127" == regex::expand("abc.127", "(?:^|\\D\\.)(\\d+)$", "0.0.0.$1"));

    auto group = regex::group("127.0.0.1", R"((\d+)\.(\d+)\.(\d+)\.(\d+))");
    CHECK(5u == group.size());
    CHECK("127" == group[1]);
    CHECK("0" == group[2]);
    CHECK("0" == group[3]);
    CHECK("1" == group[4]);
}