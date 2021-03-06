/**
 * Created by Jian Chen
 * @since  2016.03.16
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/data/json.hpp"
#include "chen/base/utf8.hpp"
#include "chen/base/str.hpp"
#include "chen/base/num.hpp"
#include "chen/sys/sys.hpp"
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <cmath>

// -----------------------------------------------------------------------------
// json
chen::json::json(json::Type type)
{
    // handy method to create default json
    switch (type)
    {
        case Type::None:
            break;

        case Type::Object:
            *this = json::object();
            break;

        case Type::Array:
            *this = json::array();
            break;

        case Type::Number:
            *this = 0;
            break;

        case Type::String:
            *this = "";
            break;

        case Type::True:
            *this = true;
            break;

        case Type::False:
            *this = false;
            break;

        default:
            *this = nullptr;
            break;
    }
}

chen::json::json(const json &o)
{
    *this = o;
}

chen::json::json(json &&o)
{
    *this = std::move(o);
}

chen::json::json(const json::object &v) : _type(Type::Object)
{
    this->_data.o = new json::object(v);
}

chen::json::json(json::object &&v) : _type(Type::Object)
{
    this->_data.o = new json::object(std::move(v));
}

chen::json::json(const json::array &v) : _type(Type::Array)
{
    this->_data.a = new json::array(v);
}

chen::json::json(json::array &&v) : _type(Type::Array)
{
    this->_data.a = new json::array(std::move(v));
}

chen::json::json(double v) : _type(Type::Number)
{
    this->_data.d = v;
}

chen::json::json(std::int32_t v) : _type(Type::Number)
{
    this->_data.d = v;
}

chen::json::json(std::uint32_t v) : _type(Type::Number)
{
    this->_data.d = v;
}

chen::json::json(const std::string &v) : _type(Type::String)
{
    this->_data.s = new std::string(v);
}

chen::json::json(std::string &&v) : _type(Type::String)
{
    this->_data.s = new std::string(std::move(v));
}

chen::json::json(const char *v) : _type(Type::String)
{
    this->_data.s = new std::string(v);
}

chen::json::json(bool v) : _type(v ? Type::True : Type::False)
{
}

chen::json::json(std::nullptr_t) : _type(Type::Null)
{
}

chen::json::~json()
{
    this->clear();
}

// assignment
chen::json& chen::json::operator=(std::nullptr_t)
{
    this->clear();

    this->_type = Type::Null;

    return *this;
}

chen::json& chen::json::operator=(const json &o)
{
    if (this == &o)
        return *this;

    this->clear();

    this->_type = o._type;

    switch (this->_type)
    {
        case Type::Object:
            this->_data.o = new json::object(*o._data.o);
            break;

        case Type::Array:
            this->_data.a = new json::array(*o._data.a);
            break;

        case Type::Number:
            this->_data.d = o._data.d;
            break;

        case Type::String:
            this->_data.s = new std::string(*o._data.s);
            break;

        default:
            break;
    }

    return *this;
}

chen::json& chen::json::operator=(json &&o)
{
    if (this == &o)
        return *this;

    this->clear();

    this->_type = o._type;
    this->_data = o._data;

    o._type = Type::None;
    o._data = {nullptr};

    return *this;
}

chen::json& chen::json::operator=(const json::object &v)
{
    if ((this->_type == Type::Object) && (this->_data.o == &v))
        return *this;

    this->clear();

    this->_type   = Type::Object;
    this->_data.o = new json::object(v);

    return *this;
}

chen::json& chen::json::operator=(json::object &&v)
{
    if ((this->_type == Type::Object) && (this->_data.o == &v))
        return *this;

    this->clear();

    this->_type   = Type::Object;
    this->_data.o = new json::object(std::move(v));

    return *this;
}

chen::json& chen::json::operator=(const json::array &v)
{
    if ((this->_type == Type::Array) && (this->_data.a == &v))
        return *this;

    this->clear();

    this->_type   = Type::Array;
    this->_data.a = new json::array(v);

    return *this;
}

chen::json& chen::json::operator=(json::array &&v)
{
    if ((this->_type == Type::Array) && (this->_data.a == &v))
        return *this;

    this->clear();

    this->_type   = Type::Array;
    this->_data.a = new json::array(std::move(v));

    return *this;
}

chen::json& chen::json::operator=(double v)
{
    this->clear();

    this->_type   = Type::Number;
    this->_data.d = v;

    return *this;
}

chen::json& chen::json::operator=(int32_t v)
{
    return *this = static_cast<double>(v);
}

chen::json& chen::json::operator=(std::uint32_t v)
{
    return *this = static_cast<double>(v);
}

chen::json& chen::json::operator=(const std::string &v)
{
    if ((this->_type == Type::String) && (this->_data.s == &v))
        return *this;

    this->clear();

    this->_type   = Type::String;
    this->_data.s = new std::string(v);

    return *this;
}

chen::json& chen::json::operator=(std::string &&v)
{
    if ((this->_type == Type::String) && (this->_data.s == &v))
        return *this;

    this->clear();

    this->_type   = Type::String;
    this->_data.s = new std::string(std::move(v));

    return *this;
}

chen::json& chen::json::operator=(const char *v)
{
    return operator=(std::string(v));
}

chen::json& chen::json::operator=(bool v)
{
    this->clear();

    this->_type = v ? Type::True : Type::False;

    return *this;
}

// parse & stringify
chen::json chen::json::parse(const std::string &text, bool file)
{
    if (file)
    {
        try
        {
            std::ifstream stream;
            stream.exceptions(std::ios::badbit | std::ios::failbit);
            stream.open(text.c_str(), std::ios_base::binary);

            std::istreambuf_iterator<char> cur(stream);
            return json::parse(cur, std::istreambuf_iterator<char>());
        }
        catch (const json::error&)
        {
            throw;
        }
        catch (const std::exception&)
        {
            // can't catch std::ios_base::failure in here
            // see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66145
            throw json::error(str::format("json: decode %s: %s", text.c_str(), sys::error().message().c_str()));
        }
    }
    else
    {
        return json::parse(text.begin(), text.end());
    }
}

chen::json chen::json::parse(iterator cur, iterator end)
{
    json item;

    const iterator beg = cur;

    // trim left spaces
    if (!json::advance(beg, cur, end, false))
        return item;

    // decode item
    json::decode(item, beg, cur, end);

    // trim right spaces
    json::advance(beg, cur, end, false);

    // should reach end
    if (cur != end)
        json::exception(beg, cur, end);

    return item;
}

std::string chen::json::stringify(const json &json, std::size_t space)
{
    std::string output;
    std::size_t indent = 0;
    json::encode(json, space, output, indent);
    return output;
}

// validate
void chen::json::validate(const std::string &text, bool file)
{
    if (file)
    {
        try
        {
            std::ifstream stream;
            stream.exceptions(std::ios::badbit | std::ios::failbit);
            stream.open(text.c_str(), std::ios_base::binary);

            std::istreambuf_iterator<char> cur(stream);
            json::validate(cur, std::istreambuf_iterator<char>());
        }
        catch (const json::error&)
        {
            throw;
        }
        catch (const std::exception&)
        {
            throw json::error(str::format("json: decode %s: %s", text.c_str(), sys::error().message().c_str()));
        }
    }
    else
    {
        json::validate(text.begin(), text.end());
    }
}

void chen::json::validate(iterator cur, iterator end)
{
    const iterator beg = cur;

    if (!json::advance(beg, cur, end, false))
        return;

    // a valid json must start by '{' or '['
    if ((*cur != '{') && (*cur != '['))
        json::exception(beg, cur, end);

    // check syntax error
    json::parse(beg, end);  // use beg, not cur, to report correct position
}

// type
chen::json::Type chen::json::type() const
{
    return this->_type;
}

// check
bool chen::json::isObject() const
{
    return this->_type == Type::Object;
}

bool chen::json::isArray() const
{
    return this->_type == Type::Array;
}

bool chen::json::isNumber() const
{
    return this->_type == Type::Number;
}

bool chen::json::isString() const
{
    return this->_type == Type::String;
}

bool chen::json::isTrue() const
{
    return this->_type == Type::True;
}

bool chen::json::isFalse() const
{
    return this->_type == Type::False;
}

bool chen::json::isNull() const
{
    return this->_type == Type::Null;
}

bool chen::json::isBool() const
{
    return (this->_type == Type::True) || (this->_type == Type::False);
}

bool chen::json::isNone() const
{
    return this->_type == Type::None;
}

// get value
const chen::json::object& chen::json::getObject() const
{
    if (this->_type == Type::Object)
        return *this->_data.o;
    else
        throw json::error("json: type is not object");
}

const chen::json::array& chen::json::getArray() const
{
    if (this->_type == Type::Array)
        return *this->_data.a;
    else
        throw json::error("json: type is not array");
}

double chen::json::getNumber() const
{
    if (this->_type == Type::Number)
        return this->_data.d;
    else
        throw json::error("json: type is not number");
}

std::int32_t chen::json::getInteger() const
{
    if (this->_type == Type::Number)
        return static_cast<std::int32_t>(this->_data.d);
    else
        throw json::error("json: type is not number");
}

std::uint32_t chen::json::getUnsigned() const
{
    if (this->_type == Type::Number)
        return static_cast<std::uint32_t>(this->_data.d);
    else
        throw json::error("json: type is not number");
}

const std::string& chen::json::getString() const
{
    if (this->_type == Type::String)
        return *this->_data.s;
    else
        throw json::error("json: type is not string");
}

bool chen::json::getBool() const
{
    if (this->_type == Type::True)
        return true;
    else if (this->_type == Type::False)
        return false;
    else
        throw json::error("json: type is not bool");
}

// operator
chen::json::operator chen::json::object() const
{
    return this->getObject();
}

chen::json::operator chen::json::array() const
{
    return this->getArray();
}

chen::json::operator double() const
{
    return this->getNumber();
}

chen::json::operator std::int8_t() const
{
    return static_cast<std::int8_t>(this->getInteger());
}

chen::json::operator std::uint8_t() const
{
    return static_cast<std::uint8_t>(this->getUnsigned());
}

chen::json::operator std::int16_t() const
{
    return static_cast<std::int16_t>(this->getInteger());
}

chen::json::operator std::uint16_t() const
{
    return static_cast<std::uint16_t>(this->getUnsigned());
}

chen::json::operator std::int32_t() const
{
    return this->getInteger();
}

chen::json::operator std::uint32_t() const
{
    return this->getUnsigned();
}

chen::json::operator std::int64_t() const
{
    return static_cast<std::int64_t>(this->getNumber());
}

chen::json::operator std::uint64_t() const
{
    return static_cast<std::uint64_t>(this->getNumber());
}

chen::json::operator std::string() const
{
    return this->getString();
}

chen::json::operator bool() const
{
    return this->getBool();
}

chen::json::object& chen::json::getObject()
{
    if (this->_type == Type::Object)
        return *this->_data.o;
    else
        throw json::error("json: type is not object");
}

chen::json::array& chen::json::getArray()
{
    if (this->_type == Type::Array)
        return *this->_data.a;
    else
        throw json::error("json: type is not array");
}

double& chen::json::getNumber()
{
    if (this->_type == Type::Number)
        return this->_data.d;
    else
        throw json::error("json: type is not number");
}

std::string& chen::json::getString()
{
    if (this->_type == Type::String)
        return *this->_data.s;
    else
        throw json::error("json: type is not string");
}

// convert value
chen::json::object chen::json::toObject() const
{
    switch (this->_type)
    {
        case Type::Object:
            return *this->_data.o;

        default:
            return json::object();
    }
}

chen::json::array chen::json::toArray() const
{
    switch (this->_type)
    {
        case Type::Array:
            return *this->_data.a;

        default:
            return json::array();
    }
}

double chen::json::toNumber() const
{
    switch (this->_type)
    {
        case Type::Number:
            return this->_data.d;

        case Type::String:
        {
            std::string &d = *this->_data.s;
            return std::atof(d.c_str());
        }

        case Type::True:
            return 1.0;

        default:
            return 0.0;
    }
}

std::int32_t chen::json::toInteger() const
{
    switch (this->_type)
    {
        case Type::Number:
            return static_cast<std::int32_t>(this->_data.d);

        case Type::String:
        {
            std::string &d = *this->_data.s;
            return std::atoi(d.c_str());
        }

        case Type::True:
            return 1;

        default:
            return 0;
    }
}

std::uint32_t chen::json::toUnsigned() const
{
    switch (this->_type)
    {
        case Type::Number:
            return static_cast<std::uint32_t>(this->_data.d);

        case Type::String:
        {
            std::string &d = *this->_data.s;
            return static_cast<std::uint32_t>(std::strtoul(d.c_str(), nullptr, 10));
        }

        case Type::True:
            return 1;

        default:
            return 0;
    }
}

std::string chen::json::toString() const
{
    switch (this->_type)
    {
        case Type::Number:
            return num::str(this->_data.d);

        case Type::String:
            return *this->_data.s;

        case Type::True:
            return "true";

        case Type::False:
            return "false";

        case Type::Null:
            return "null";

        default:
            return "";
    }
}

bool chen::json::toBool() const
{
    switch (this->_type)
    {
        // treat object as true, like javascript
        case Type::Object:
        case Type::Array:
            return true;

        case Type::Number:
            return !num::equal(this->_data.d, 0.0);

        case Type::String:
            return !this->_data.s->empty();

        case Type::True:
            return true;

        default:
            return false;
    }
}

// clear
void chen::json::clear()
{
    switch (this->_type)
    {
        case Type::Object:
            delete this->_data.o;
            break;

        case Type::Array:
            delete this->_data.a;
            break;

        case Type::String:
            delete this->_data.s;
            break;

        default:
            break;
    }

    this->_type = Type::None;
    this->_data = {nullptr};
}

// exception
void chen::json::exception(const iterator &beg, iterator &cur, iterator &end)
{
    if (cur == end)
    {
        throw json::error("json: unexpected end of input");
    }
    else
    {
        auto pos = cur.distance();
        auto tok = std::isprint(*cur) ? std::string(1, *cur) : str::format("\\x%02x", *cur);
        throw json::error(str::format("json: unexpected token '%s' at position %lu", tok.c_str(), pos), pos);
    }
}

// advance
bool chen::json::advance(const iterator &beg, iterator &cur, iterator &end, bool require)
{
    // skip whitespaces
    while ((cur != end) && std::isspace(*cur))
        ++cur;

    // check if end
    if (cur == end)
    {
        if (require)
            json::exception(beg, cur, end);
        else
            return false;
    }

    return true;
}

// decode type
void chen::json::decode(chen::json &out, const iterator &beg, iterator &cur, iterator &end)
{
    json::advance(beg, cur, end);

    switch (*cur)
    {
        case '{':  // object
        {
            json::object o;
            json::decode(o, beg, cur, end);
            out = std::move(o);
        }
            break;

        case '[':  // array
        {
            json::array a;
            json::decode(a, beg, cur, end);
            out = std::move(a);
        }
            break;

        case '"':  // string
        {
            std::string s;
            json::decode(s, beg, cur, end);
            out = std::move(s);
        }
            break;

        case 't':  // true
            json::decode(true, beg, cur, end);
            out = true;
            break;

        case 'f':  // false
            json::decode(false, beg, cur, end);
            out = false;
            break;

        case 'n':  // null
            json::decode(nullptr, beg, cur, end);
            out = nullptr;
            break;

        default:
            if (std::isdigit(*cur) || (*cur == '-'))  // number
            {
                double d = 0;
                json::decode(d, beg, cur, end);
                out = d;
            }
            else
            {
                json::exception(beg, cur, end);
            }
            break;
    }
}

void chen::json::decode(json::object &out, const iterator &beg, iterator &cur, iterator &end)
{
    if ((cur == end) || (*cur != '{'))
        json::exception(beg, cur, end);

    json::advance(beg, ++cur, end);

    while (cur != end)
    {
        // skip spaces
        json::advance(beg, cur, end);

        if (*cur == '}')
            break;

        // find key
        if (*cur != '"')
            json::exception(beg, cur, end);

        std::string key;
        json::decode(key, beg, cur, end);

        // find separator
        json::advance(beg, cur, end);

        if (*cur != ':')  // separator
            json::exception(beg, cur, end);

        json::advance(beg, ++cur, end);

        // find value
        json item;
        json::decode(item, beg, cur, end);

        out[std::move(key)] = std::move(item);  // override if key already exist

        // find comma or ending
        json::advance(beg, cur, end);

        if (*cur == ',')
        {
            json::advance(beg, ++cur, end);

            // don't allow json like {"k":1,}
            if (*cur == '}')
                json::exception(beg, cur, end);
        }
        else if (*cur == '}')
        {
            break;
        }
        else
        {
            json::exception(beg, cur, end);
        }
    }

    if (cur == end)
        json::exception(beg, cur, end);

    ++cur;
}

void chen::json::decode(json::array &out, const iterator &beg, iterator &cur, iterator &end)
{
    if ((cur == end) || (*cur != '['))
        json::exception(beg, cur, end);

    json::advance(beg, ++cur, end);

    while (cur != end)
    {
        if (*cur == ']')
            break;

        // find value
        json item;
        json::decode(item, beg, cur, end);

        out.emplace_back(std::move(item));

        // find comma or ending
        json::advance(beg, cur, end);

        if (*cur == ',')
        {
            json::advance(beg, ++cur, end);

            // don't allow json like [1,]
            if (*cur == ']')
                json::exception(beg, cur, end);
        }
        else if (*cur == ']')
        {
            break;
        }
        else
        {
            json::exception(beg, cur, end);
        }
    }

    if (cur == end)
        json::exception(beg, cur, end);

    ++cur;
}

void chen::json::decode(double &out, const iterator &beg, iterator &cur, iterator &end)
{
    if (cur == end)
        json::exception(beg, cur, end);

    std::string str(1, *cur++);

    // must begin with '-' or '1' ~ '9' or like '0.123456'
    if (str[0] == '-')
    {
        if ((cur == end) || !std::isdigit(*cur))
        {
            json::exception(beg, cur, end);
        }
        else if (*cur == '0')
        {
            str += *cur++;

            if ((cur != end) && std::isdigit(*cur))
                json::exception(beg, cur, end);
        }
    }
    else if ((str[0] == '0') && std::isdigit(*cur))
    {
        // if the first char is '0', then the whole number must be 0
        json::exception(beg, cur, end);
    }

    // collect integer parts
    while ((cur != end) && std::isdigit(*cur))
        str += *cur++;

    // collect decimal parts
    if ((cur != end) && (*cur == '.'))
    {
        str += *cur++;

        // first
        if ((cur != end) && std::isdigit(*cur))
            str += *cur++;
        else
            json::exception(beg, cur, end);

        // digits
        while ((cur != end) && std::isdigit(*cur))
            str += *cur++;
    }

    // 'e' or 'E'
    if ((cur != end) && ((*cur == 'e') || (*cur == 'E')))
    {
        str += *cur++;

        // '+', '-' or digits
        if ((cur != end) && ((*cur == '+') || (*cur == '-') || std::isdigit(*cur)))
        {
            str += *cur++;

            // first is '+' or '-'
            if (!std::isdigit(str.back()))
            {
                if ((cur != end) && std::isdigit(*cur))
                    str += *cur++;
                else
                    json::exception(beg, cur, end);
            }

            // last
            while ((cur != end) && std::isdigit(*cur))
                str += *cur++;
        }
        else
        {
            json::exception(beg, cur, end);
        }
    }

    // check if number is overflow
    double d = std::atof(str.c_str());

    if (std::isinf(d))
    {
        auto pos = cur.distance() - str.size();
        throw json::error(str::format("json: number '%s' is overflow at position %lu", str.c_str(), pos), pos);
    }
    else if (std::isnan(d))
    {
        auto pos = cur.distance() - str.size();
        throw json::error(str::format("json: number '%s' is invalid at position %lu", str.c_str(), pos), pos);
    }

    out = d;
}

void chen::json::decode(std::string &out, const iterator &beg, iterator &cur, iterator &end)
{
    out.clear();

    if ((cur == end) || (*cur != '"') || (++cur == end))
        json::exception(beg, cur, end);

    char ch = *cur;

    while (ch != '"')
    {
        // control characters must use escape
        if ((ch >= 0) && (ch <= 31))  // see ASCII
        {
            auto pos = cur.distance();
            throw json::error(str::format("json: control character code '%d' is not escaped at position %lu", static_cast<int>(ch), pos), pos);
        }

        // unescape characters
        if (ch == '\\')
        {
            if (++cur == end)
                json::exception(beg, cur, end);
            else
                ch = *cur;

            switch (ch)
            {
                case '"':
                case '\\':
                case '/':
                    out += ch;
                    break;

                case 'b':
                    out += '\b';
                    break;

                case 'f':
                    out += '\f';
                    break;

                case 'n':
                    out += '\n';
                    break;

                case 'r':
                    out += '\r';
                    break;

                case 't':
                    out += '\t';
                    break;

                case 'u':
                {
                    // handle unicode character
                    char unicode[5];
                    unicode[4] = '\0';

                    for (auto i = 0; i < 4; ++i)
                    {
                        if (++cur == end)
                            json::exception(beg, cur, end);

                        ch = *cur;

                        if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f')) || ((ch >= 'A') && (ch <= 'F')))
                            unicode[i] = ch;  // char must in range of '0' ~ '9', 'a' ~ 'f', 'A' ~ 'F'
                        else
                            json::exception(beg, cur, end);
                    }

                    // convert utf-16 to utf-8
                    try
                    {
                        out.append(utf8::convert(static_cast<std::uint32_t>(std::strtol(unicode, nullptr, 16))));
                    }
                    catch (...)
                    {
                        // e.g: \uD83D\uDE00, it's a emoji character
                        auto pos = cur.distance() - 4;
                        throw json::error(str::format("json: invalid unicode char '\\u%s' at position %lu", unicode, pos), pos);
                    }
                }
                    break;

                default:
                    json::exception(beg, cur, end);
            }
        }
        else
        {
            out += ch;
        }

        if (++cur == end)
            json::exception(beg, cur, end);
        else
            ch = *cur;
    }

    ++cur;
}

void chen::json::decode(bool v, const iterator &beg, iterator &cur, iterator &end)
{
    constexpr char t[] = "true";
    constexpr char f[] = "false";

    const char *s = v ? t : f;
    const auto  l = v ? sizeof(t) - 1 : sizeof(f) - 1;

    for (std::size_t i = 0; i < l; ++i, ++cur)
    {
        if ((cur == end) || (*cur != s[i]))
            json::exception(beg, cur, end);
    }
}

void chen::json::decode(std::nullptr_t, const iterator &beg, iterator &cur, iterator &end)
{
    constexpr char n[] = "null";
    constexpr auto len = sizeof(n) - 1;

    for (std::size_t i = 0; i < len; ++i, ++cur)
    {
        if ((cur == end) || (*cur != n[i]))
            json::exception(beg, cur, end);
    }
}

// encode type
void chen::json::encode(const json &v, std::size_t space, std::string &output, std::size_t &indent)
{
    switch (v.type())
    {
        case Type::None:
            break;

        case Type::Object:
            return json::encode(v.getObject(), space, output, indent);

        case Type::Array:
            return json::encode(v.getArray(), space, output, indent);

        case Type::Number:
            return json::encode(v.getNumber(), output);

        case Type::String:
            return json::encode(v.getString(), output);

        case Type::True:
        case Type::False:
            return json::encode(v.getBool(), output);

        case Type::Null:
            return json::encode(nullptr, output);
    }
}

void chen::json::encode(const json::object &v, std::size_t space, std::string &output, std::size_t &indent)
{
    output += '{';

    if (space && !v.empty())
    {
        indent += space;
        output += '\n';
    }

    auto beg = v.begin();
    auto end = v.end();

    for (auto it = beg; it != end; ++it)
    {
        auto &key = it->first;
        auto &val = it->second;

        if (it != beg)
        {
            output += ',';

            if (space)
                output += '\n';
        }

        if (indent)
            output.append(indent, ' ');

        output += '"';
        output.append(key);
        output.append("\":");

        if (space)
            output += ' ';

        json::encode(val, space, output, indent);
    }

    if (space && !v.empty())
    {
        indent -= space;
        output += '\n';
        output.append(indent, ' ');
    }

    output += '}';
}

void chen::json::encode(const json::array &v, std::size_t space, std::string &output, std::size_t &indent)
{
    output += '[';

    if (space && !v.empty())
    {
        indent += space;
        output += '\n';
    }

    for (std::size_t i = 0, len = v.size(); i < len; ++i)
    {
        if (i)
        {
            output += ',';

            if (space)
                output += '\n';
        }

        if (indent)
            output.append(indent, ' ');

        json::encode(v[i], space, output, indent);
    }

    if (space && !v.empty())
    {
        indent -= space;
        output += '\n';
        output.append(indent, ' ');
    }

    output += ']';
}

void chen::json::encode(double v, std::string &output)
{
    output.append(num::str(v));
}

void chen::json::encode(const std::string &v, std::string &output)
{
    // escape the control characters only
    // json can accept unicode chars, so we don't escape unicode
    // see http://www.json.org
    output += '"';

    for (auto &ch : v)
    {
        switch (ch)
        {
            case '"':
                output.append("\\\"");
                break;

            case '\\':
                output.append("\\\\");
                break;

            case '/':
                output.append("\\/");
                break;

            case '\b':
                output.append("\\b");
                break;

            case '\f':
                output.append("\\f");
                break;

            case '\n':
                output.append("\\n");
                break;

            case '\r':
                output.append("\\r");
                break;

            case '\t':
                output.append("\\t");
                break;

            default:
                output += ch;
                break;
        }
    }

    output += '"';
}

void chen::json::encode(bool v, std::string &output)
{
    output.append(v ? "true" : "false");
}

void chen::json::encode(std::nullptr_t, std::string &output)
{
    output.append("null");
}