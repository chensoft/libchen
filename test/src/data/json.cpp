/**
 * Created by Jian Chen
 * @since  2016.06.17
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/data/json.hpp"
#include "chen/base/str.hpp"
#include "chen/sys/fs.hpp"
#include "catch.hpp"

TEST_CASE("DataJsonTest")
{
    using chen::json;
    using chen::str;
    using chen::fs;

    SECTION("Type")
    {
        json json_object(json::Type::Object);
        json json_array(json::Type::Array);
        json json_number(json::Type::Number);
        json json_string(json::Type::String);
        json json_true(json::Type::True);
        json json_false(json::Type::False);

        json::object unused_o = json_object;
        json::array unused_a  = json_array;

        double unused_d = json_number;
        std::int8_t   unused_i8  = json_number;
        std::uint8_t  unused_u8  = json_number;
        std::int16_t  unused_i16 = json_number;
        std::uint16_t unused_u16 = json_number;
        std::int32_t  unused_i32 = json_number;
        std::uint32_t unused_u32 = json_number;
        std::int64_t  unused_i64 = json_number;
        std::uint64_t unused_u64 = json_number;

        std::string unused_s = json_string;
        bool unused_b = json_true;

        CHECK(0.0 == unused_d);
        CHECK(0 == unused_i8);
        CHECK(0 == unused_u8);
        CHECK(0 == unused_i16);
        CHECK(0 == unused_u16);
        CHECK(0 == unused_i32);
        CHECK(0u == unused_u32);
        CHECK(0 == unused_i64);
        CHECK(0u == unused_u64);
        CHECK(true == unused_b);

        json_object.getObject().clear();
        json_array.getArray().clear();
        json_number.getNumber() = 115;
        json_string.getString() = "123";

        CHECK(json_object.toObject().empty());
        CHECK(json_array.toArray().empty());

        CHECK(json(unused_o).getObject().empty());
        CHECK(json(unused_a).getArray().empty());
        CHECK(json(unused_s).getString().empty());

        json assign_o;
        json assign_a;
        json assign_s;
        json assign_d;

        assign_o = unused_o;
        assign_a = unused_a;
        assign_s = unused_s;
        assign_d = 115u;

        CHECK(assign_o.getObject().empty());
        CHECK(assign_a.getArray().empty());
        CHECK(assign_s.getString().empty());
        CHECK(115u == assign_d.getUnsigned());

        CHECK(123 == json_string.toNumber());
        CHECK(123 == json_string.toInteger());
        CHECK(123u == json_string.toUnsigned());
        CHECK("115" == json_number.toString());
        CHECK(json_object.toBool());

        CHECK(json(json::Type::None).isNone());
        CHECK(json_object.isObject());
        CHECK(json_array.isArray());
        CHECK(json_number.isNumber());
        CHECK(json_string.isString());
        CHECK(json_true.isTrue());
        CHECK(json_false.isFalse());
        CHECK(json(json::Type::Null).isNull());

        CHECK(json().isNone());
        CHECK(json(json::object()).isObject());
        CHECK(json(json::array()).isArray());
        CHECK(json(115).isNumber());
        CHECK(json(115u).isNumber());
        CHECK(json(0.715002586).isNumber());
        CHECK(json("Jian Chen").isString());
        CHECK(json(std::string("Jian Chen")).isString());
        CHECK(json(true).isTrue());
        CHECK(json(false).isFalse());
        CHECK(json(true).isBool());
        CHECK(json(nullptr).isNull());

        // parse special string
        CHECK(json::parse("   ").isNone());
        CHECK_THROWS_AS(json::parse("-"), json::error);
        CHECK_THROWS_AS(json::parse("-03"), json::error);
        CHECK_THROWS_AS(json::parse("-0."), json::error);
        CHECK_THROWS_AS(json::parse("1e123456789"), json::error);
        CHECK_THROWS_AS(json::parse("\"ab\\"), json::error);
        CHECK_THROWS_AS(json::parse("\"ab\\u9A"), json::error);
        CHECK_THROWS_AS(json::parse("\"ab\\u9A@@"), json::error);
        CHECK_THROWS_AS(json::parse("\"\\uD83D\\uDE00\""), json::error);
        CHECK_THROWS_AS(json::parse("nul"), json::error);
    }

    SECTION("Validate")
    {
        auto dir = fs::dirname(__FILE__) + "/../../data/";

        // fail
        for (int i = 1; i <= 33; ++i)
        {
            if (i == 18)  // I don't think too deep is an error
                continue;

            CHECK_THROWS_AS(json::validate(dir + str::format("json/fail%d.json", i), true), json::error);
        }

        CHECK_THROWS_AS(json::validate(dir + "json/role.json", true), json::error);

        // pass
        for (int j = 1; j <= 3; ++j)
        {
            CHECK_NOTHROW(json::validate(dir + str::format("json/pass%d.json", j), true));
        }

        CHECK_NOTHROW(json::parse(dir + "json/pass1.json", true));

        // exist
        CHECK_THROWS_AS(json::parse(dir + "json/not_exist.json", true), json::error);

        // equal
        std::string text = R"([
    1,
    2,
    3,
    false,
    null,
    {
        "key": "val-\"\\\/\b\f\n\r\tchars"
    }
])";

        auto parse = json::parse(text);
        CHECK(text == json::stringify(parse, 4));

        // error
        text = "{unquoted_key: \"keys must be quoted\"}";
        CHECK_THROWS_AS(json::parse(text), json::error);

        try
        {
            json::parse(text);
        }
        catch (const json::error &e)
        {
            CHECK(1u == e.position);
        }
    }
}