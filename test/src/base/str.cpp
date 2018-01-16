/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/str.hpp"
#include "catch.hpp"

TEST_CASE("BaseStrTest")
{
    using chen::str;

    // format
    CHECK("" == str::format(""));
    CHECK("plain" == str::format("plain"));
    CHECK("year 2016" == str::format("year %d", 2016));
    CHECK(8u == str::format("chen%s", "jian").size());

    // check
    CHECK(str::equal("chen", 4, "chen", 4));
    CHECK(str::prefix("chensoft", "chen"));
    CHECK_FALSE(str::prefix("chensoft", "soft"));
    CHECK_FALSE(str::prefix("chen", "chensoft"));
    CHECK(str::suffix("chensoft", "soft"));
    CHECK_FALSE(str::suffix("chensoft", "chen"));
    CHECK_FALSE(str::suffix("chen", "chensoft"));
    CHECK(str::contain("libchen", "lib"));

    // count
    CHECK(3u == str::count("I scream, you scream, we all scream for ice-cream!", "scream"));

    // split & join
    std::vector<std::string> split({"10", "0", "0", "1"});
    CHECK(split == str::split("10.0.0.1", "."));
    CHECK(split == str::split("10->0->0->1", "->"));
    CHECK(std::vector<std::string>{"10"} == str::split("10", "."));
    CHECK(std::vector<std::string>{} == str::split("", "."));
    CHECK("10.0.0.1" == str::join(split, "."));
    CHECK("" == str::join(std::vector<std::string>(), "."));

    // replace
    CHECK("I'm a replace string" == str::replace("I'm a search string", "search", "replace"));
    CHECK("I'm a  string" == str::remove("I'm a replace string", "replace"));

    CHECK("I'm a replace string, and another search string" == str::replace("I'm a search string, and another search string", "search", "replace", false));
    CHECK("I'm a replace string, and another replace string" == str::replace("I'm a search string, and another search string", "search", "replace"));

    // trim
    CHECK("so many spaces  " == str::ltrim("  so many spaces  "));
    CHECK("  so many spaces" == str::rtrim("  so many spaces  "));
    CHECK("so many spaces" == str::trim("  so many spaces  "));

    // transform
    CHECK("" == str::capitalize(""));
    CHECK("AbCDe" == str::capitalize("abCDe"));
    CHECK("abcde" == str::lowercase("abCDe"));
    CHECK("ABCDE" == str::uppercase("abCDe"));

    // levenshtein
    CHECK(2u == str::levenshtein("pull", 4, "push", 4));
}