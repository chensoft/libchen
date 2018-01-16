/**
 * Created by Jian Chen
 * @since  2016.06.06
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/sys/sys.hpp"
#include "catch.hpp"

TEST_CASE("SysSysTest")
{
    using chen::sys;

    CHECK(sys::uuid() != sys::uuid());
}