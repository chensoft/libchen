/**
  * Created by Jian Chen
  * @since  2016.05.27
  * @author Jian Chen <admin@chensoft.com>
  * @link   http://chensoft.com
  */
#include "chen/base/utf8.hpp"
#include "chen/sys/fs.hpp"
#include "catch.hpp"

TEST_CASE("BaseUtf8Test")
{
    using chen::fs;
    using chen::utf8;

    // these txt files contains utf-8 chars, each char's size is 1byte ~ 6byte
    auto dir   = fs::dirname(__FILE__) + "/../../data/";
    auto bom   = fs::read(dir + "utf8/bom.txt");
    auto sub   = fs::read(dir + "utf8/sub.txt");
    auto code  = fs::read(dir + "utf8/code.txt");
    auto plain = fs::read(dir + "utf8/plain.txt");

    CHECK(11u == utf8::count(plain));
    CHECK(sub == utf8::substr(plain, 0, 7));
    CHECK("" == utf8::substr(plain, 5, 0));
    CHECK(utf8::bom(bom));
    CHECK_FALSE(utf8::bom(plain));

    CHECK("a" == utf8::convert(0x0061));
    CHECK("©" == utf8::convert(0x00a9));
    CHECK(code == utf8::convert(0x9648));
}