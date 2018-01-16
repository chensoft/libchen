/**
 * Created by Jian Chen
 * @since  2016.05.20
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#define CATCH_CONFIG_RUNNER

#include "chen/chen.hpp"
#include "catch.hpp"

int main(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}