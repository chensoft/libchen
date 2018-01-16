/**
 * Created by Jian Chen
 * @since  2016.06.17
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/data/ini.hpp"
#include "chen/base/str.hpp"
#include "chen/sys/fs.hpp"
#include "catch.hpp"

TEST_CASE("DataIniTest")
{
    using chen::ini;
    using chen::str;
    using chen::fs;

    auto dir = fs::dirname(__FILE__) + "/../../data/";

    // fail
    for (int i = 1; i <= 5; ++i)
    {
        CHECK_THROWS_AS(ini::parse(dir + str::format("ini/fail%d.ini", i), true), ini::error);
    }

    // pass
    for (int j = 1; j <= 4; ++j)
    {
        CHECK_NOTHROW(ini::parse(dir + str::format("ini/pass%d.ini", j), true));
    }

    // equal(usage of block text in C++11)
    std::string text = R"([section]
key="simple \\\0\a\b\t\r\n\:\"\;\#value")";

    auto parse = ini::parse(text);
    CHECK(text == ini::stringify(parse));

    // parse special string
    CHECK(ini::parse("   ").empty());
    CHECK_THROWS_AS(ini::parse("ab=\\x9A"), ini::error);
    CHECK_THROWS_AS(ini::parse("ab=\\x9A@@"), ini::error);
    CHECK_THROWS_AS(ini::parse("emoji=\\xD83D\\xDE00"), ini::error);

    // error
    text = R"([section
key="section is not enclosed")";

    CHECK_THROWS_AS(ini::parse(text), ini::error);

    try
    {
        ini::parse(text);
    }
    catch (const ini::error &e)
    {
        CHECK(8u == e.position);
    }
}