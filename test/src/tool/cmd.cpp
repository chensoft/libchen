/**
 * Created by Jian Chen
 * @since  2016.06.06
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/tool/cmd.hpp"
#include "catch.hpp"

TEST_CASE("ToolCmdTest")
{
    chen::cmd cmd;

    // help
    cmd.create("help", "show help");
    cmd.create("help", "show app help");  // test overwrite

    // version
    cmd.create("version", "show app version");

    // sub-action
    cmd.create("module", "operate module");
    cmd.create("module.update", "update module info");

    // start
    cmd.create("start", "start server");
    cmd.define("port", "p", "server port (default: 80)", 80);
    cmd.define("addr", "a", "server address (default: 0.0.0.0)", "0.0.0.0");
    cmd.define("speed", "s", "speed limit", 1);
    cmd.define("thread", "t", "thread count (default: 1)", 1);
    cmd.define("daemon", "d", "run as a daemon (default: no)", false);

    CHECK_THROWS_AS(cmd.define("", "", "", ""), chen::cmd::error_general);
    CHECK_THROWS_AS(cmd.define("s", "", "", ""), chen::cmd::error_general);
    CHECK_THROWS_AS(cmd.define("listen", "tiny", "", ""), chen::cmd::error_general);
    CHECK_THROWS_AS(cmd.define("port", "", "", ""), chen::cmd::error_general);
    CHECK_THROWS_AS(cmd.define("listen", "p", "", ""), chen::cmd::error_general);

    CHECK_NOTHROW(cmd.change("start"));
    CHECK_THROWS_AS(cmd.change("noaction"), chen::cmd::error_general);

    CHECK_FALSE(cmd.exist("noaction"));
    CHECK_FALSE(cmd.exist("start", "nooption"));

    // suggest
    cmd.suggest("edition", "version");

    // simulate -> version
    char param_app[] = "app";
    char param_version[] = "version";

    {
        char *argv[] = {param_app, param_version, nullptr};

        cmd.parse(sizeof(argv) / sizeof(argv[0]) - 1, &argv[0]);

        CHECK(static_cast<int>(sizeof(argv) / sizeof(argv[0])) - 1 == cmd.argc());
        CHECK(&argv[0] == cmd.argv());
        CHECK("app" == cmd.app());
        CHECK("version" == cmd.current());
    }

    // simulate -> start
    char param_start[] = "start";

    {
        char *argv[] = {param_app, param_start, nullptr};

        cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]);

        CHECK("start" == cmd.current());
        CHECK(80 == cmd.intVal("port"));
        CHECK("0.0.0.0" == cmd.strVal("addr"));

        CHECK_FALSE(cmd.isSet("daemon"));
        CHECK_FALSE(cmd.boolVal("daemon"));
    }

    // simulate -> start
    char param_port[]   = "--port=8888";
    char param_addr[]   = "-a=127.0.0.1";
    char param_thread[] = "--thread";
    char param_four[]   = "4";
    char param_s[]      = "-s";
    char param_100[]    = "100";
    char param_ip[]     = "127.0.0.1";

    {
        char *argv[] = {param_app, param_start, param_port, param_addr, param_thread, param_four, param_s, param_100, param_ip, nullptr};

        cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]);

        CHECK("start" == cmd.current());
        CHECK(8888 == cmd.intVal("port"));
        CHECK("127.0.0.1" == cmd.strVal("addr"));

        CHECK_THROWS_AS(cmd.strVal("x"), chen::cmd::error);
        CHECK_THROWS_AS(cmd.strVal("color"), chen::cmd::error);

        CHECK(cmd.isSet("port"));
        CHECK_FALSE(cmd.objects().empty());
    }

    // simulate -> sub-action
    char param_module[] = "module";
    char param_update[] = "update";

    {
        char *argv[] = {param_app, param_module, param_update, nullptr};

        cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]);

        CHECK("module.update" == cmd.current());
    }

    // simulate -> exception
    char param_a[] = "--a";  // it's not a long option name

    {
        char *argv[] = {param_app, param_start, param_a, nullptr};

        CHECK_THROWS_AS(cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]), chen::cmd::error);
    }

    char param_speedx[] = "--speedx";  // long option not exist

    {
        char *argv[] = {param_app, param_start, param_speedx, nullptr};

        CHECK_THROWS_AS(cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]), chen::cmd::error);
    }

    char param_x[] =  "-x";  // short option not exist

    {
        char *argv[] = {param_app, param_start, param_x, nullptr};

        CHECK_THROWS_AS(cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]), chen::cmd::error);
    }


    char param_editi[] = "editi";

    {
        char *argv[] = {param_app, param_editi, nullptr};

        try
        {
            cmd.parse(static_cast<int>(sizeof(argv) / sizeof(argv[0]) - 1), &argv[0]);
            CHECK_NOTHROW(throw "should not reach here");
        }
        catch (const chen::cmd::error_parse &error)
        {
            cmd.usage(error);
        }

        // just call
        cmd.usage();
        cmd.usage("start");
        cmd.usage("start", "");
        cmd.usage("start", "port");

        cmd.visit([] (const chen::cmd::action &action, std::size_t idx, std::size_t len) {
        });

        cmd.visit("start", [] (const chen::cmd::option &option, std::size_t idx, std::size_t len) {
        });
    }
}