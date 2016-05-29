/**
 * Created by Jian Chen
 * @since  2016.05.27
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#pragma once

#include <chen/chen.hpp>
#include <gtest/gtest.h>

TEST(BaseNumTest, General)
{
    // equal
    EXPECT_TRUE(chen::num::equal(0.1, 0.1));
    EXPECT_TRUE(chen::num::equal(0.1f, 0.100000001490116119715002586f));
    EXPECT_TRUE(chen::num::equal(0.1, 0.1000000000000000055511151231257001365183510300715002586));

    // swap
    EXPECT_EQ(static_cast<std::uint16_t>(4660), chen::num::swap(static_cast<std::uint16_t>(13330)));
    EXPECT_EQ(static_cast<std::uint32_t>(305419896), chen::num::swap(static_cast<std::uint32_t>(2018915346)));

    // random
    for (int i = 0; i < 1000; ++i)
    {
        auto real = chen::num::random();

        EXPECT_GE(real, 0.0);
        EXPECT_LT(real, 1.0);

        auto num = chen::num::random(0, 9);

        EXPECT_GE(num, 0);
        EXPECT_LE(num, 9);
    }
}