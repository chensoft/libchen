/**
 * Created by Jian Chen
 * @since  2016.07.18
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/base/iterator.hpp"
#include "catch.hpp"
#include <array>
#include <list>

TEST_CASE("BaseIterTest")
{
    using chen::input_iterator;
    using chen::forward_iterator;
    using chen::bidirectional_iterator;
    using chen::random_iterator;

    // plain text
    char buf[] = "chen";
    random_iterator<char> iter(buf);

    CHECK('c' == *iter);
    CHECK('c' == *iter++);
    CHECK('h' == *iter);
    CHECK('e' == *++iter);
    CHECK('h' == *--iter);
    CHECK('h' == *iter);
    CHECK('h' == *iter--);
    CHECK('c' == *iter);

    CHECK('c' == iter[0]);
    CHECK('n' == iter[3]);

    CHECK('n' == *(iter + 3));

    iter += 3;
    CHECK('n' == *iter);

    CHECK('c' == *(iter - 3));

    iter -= 3;
    CHECK('c' == *iter);

    random_iterator<char> proxy(iter++);
    CHECK('c' == *proxy);

    random_iterator<char> other(iter);

    CHECK(iter == other);
    CHECK(iter <= other);
    CHECK_FALSE(iter != other);

    ++other;
    CHECK(iter < other);
    CHECK(iter <= other);
    CHECK_FALSE(iter > other);
    CHECK_FALSE(iter >= other);

    // vector and array
    std::vector<char> vector(buf, buf + ::strlen(buf));
    std::array<char, 4> array{};

    input_iterator<char> cur(vector.begin());
    input_iterator<char> end(vector.end());

    std::copy(cur, end, array.begin());
    CHECK(std::equal(vector.begin(), vector.end(), array.begin()));

    // list, its iterator is a bidirectional iterator
    std::list<char> list{'c', 'h', 'e', 'n'};

    bidirectional_iterator<char> a(list.begin());
    bidirectional_iterator<char> b(list.end());

    forward_iterator<char> f(buf);

    while (a != b)
        CHECK(*a++ == *f++);
}