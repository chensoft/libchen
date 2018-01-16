/**
 * Created by Jian Chen
 * @since  2016.06.06
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#include "chen/sys/fs.hpp"
#include "catch.hpp"

TEST_CASE("SysFsTest")
{
    using chen::fs;

    // path
    fs::home();
    fs::temp();

    // drive
    CHECK("C:\\" == fs::drive("C:\\Windows\\System32"));
    CHECK("/" == fs::drive("/usr/local"));
    CHECK("" == fs::drive("file.txt"));
    CHECK("" == fs::drive(""));

    // path
    CHECK(fs::current() == fs::realpath(fs::current()));
    CHECK(fs::current() == fs::absolute(".", fs::current()));

    CHECK(fs::home() == fs::realpath("~"));
    CHECK(fs::home() == fs::absolute("~"));
    CHECK(fs::home() + "/Downloads" == fs::absolute("~/Downloads"));

    // normalize
    CHECK("" == fs::normalize(""));
    CHECK("a" == fs::normalize("./a"));
    CHECK("a/b" == fs::normalize("a/./b"));
    CHECK("a/b" == fs::normalize("a///b"));
    CHECK("a/.../b" == fs::normalize("a/.../b"));  // this is a invalid path
    CHECK("../b" == fs::normalize("a/../../b"));   // the second .. don't know how to removed
    CHECK("/usr/local" == fs::normalize("/usr/local/etc/.."));
    CHECK("/" == fs::normalize("/.."));

    CHECK("C:\\a" == fs::normalize("C:\\a"));
    CHECK("C:\\a" == fs::normalize("C:\\.\\a"));
    CHECK("C:\\a\\...\\b" == fs::normalize("C:\\a\\...\\b"));
    CHECK("C:\\b" == fs::normalize("C:\\a\\..\\..\\b"));
    CHECK("C:\\b" == fs::normalize("C:\\a\\..\\b"));

    // dirname
    CHECK("" == fs::dirname(""));
    CHECK("/home/staff/Downloads" == fs::dirname("/home/staff/Downloads/file.txt"));
    CHECK("/usr" == fs::dirname("/usr/."));
    CHECK("/" == fs::dirname("/usr/"));
    CHECK("/" == fs::dirname("/usr///"));
    CHECK("/" == fs::dirname("/"));
    CHECK("." == fs::dirname("file.txt"));

    CHECK("C:\\Windows" == fs::dirname("C:\\Windows\\System32"));
    CHECK("C:\\Windows\\System32" == fs::dirname("C:\\Windows\\System32\\cmd.exe"));
    CHECK("C:\\" == fs::dirname("C:\\\\\\"));
    CHECK("C:\\" == fs::dirname("C:\\"));

    // basename
    CHECK("file.txt" == fs::basename("/home/staff/Downloads/file.txt"));
    CHECK("home" == fs::basename("/home/"));
    CHECK("" == fs::basename("/"));
    CHECK("file.txt" == fs::basename("file.txt"));
    CHECK("file.txt" == fs::basename("file.txt", "none"));
    CHECK("file" == fs::basename("file.txt", ".txt"));

    CHECK("cmd.exe" == fs::basename("C:\\Windows\\System32\\cmd.exe"));
    CHECK("" == fs::basename("C:\\"));

    // extname
    CHECK("" == fs::extname("file.txt", 0));
    CHECK(".txt" == fs::extname("/home/staff/Downloads/file.txt"));
    CHECK("" == fs::extname("/home/"));
    CHECK("" == fs::extname("/"));

    CHECK(".exe" == fs::extname("C:\\Windows\\System32\\cmd.exe"));
    CHECK("" == fs::extname("C:\\"));

    // absolute
    CHECK(fs::isAbsolute("/usr/local"));
    CHECK_FALSE(fs::isAbsolute("file.txt"));
    CHECK_FALSE(fs::isAbsolute(""));

    CHECK(fs::isAbsolute("C:\\Windows\\System32"));

    // change
    auto cwd = fs::current();
    fs::change(cwd);
    CHECK(cwd == fs::current());

    // platform dependent
#ifndef _WIN32
    CHECK("/" == fs::root());
    CHECK(std::vector<std::string>({"/"}) == fs::drives());
    CHECK('/' == fs::separator());

    // absolute
    CHECK(fs::normalize(fs::current() + "/../a/b") == fs::absolute("../a/b"));
    CHECK("/a/b" == fs::absolute("/a/b"));
    CHECK("/a/b" == fs::absolute("/a/b", fs::current()));

    // check
    CHECK(fs::isExist("/"));
    CHECK(fs::isDir("/"));
    CHECK_FALSE(fs::isFile("/"));
    CHECK_FALSE(fs::isLink("/"));

    // just call
    fs::isReadable("/");
    fs::isWritable("/");
    fs::isExecutable("/");

    fs::atime("/");
    fs::mtime("/");
    fs::ctime("/");

    // touch
    fs::write("/tmp/libchen-test.tmp", "content");
    fs::write("/tmp/libchen-test.tmp", "content", 7);
    fs::append("/tmp/libchen-test.tmp", "content");
    fs::append("/tmp/libchen-test.tmp", "content", 7);
    fs::read("/tmp/libchen-test.tmp", '\n');

    fs::touch("/tmp/libchen-test.tmp");
    fs::touch("/tmp/libchen-test.tmp", std::time(nullptr), std::time(nullptr));
    fs::copy("/tmp/libchen-test.tmp", "/tmp/libchen-test1.tmp");
    fs::rename("/tmp/libchen-test.tmp", "/tmp/libchen-test2.tmp");
    fs::remove("/tmp/libchen-test.tmp");
    fs::remove("/tmp/libchen-test1.tmp");
    fs::remove("/tmp/libchen-test2.tmp");

    fs::create("/tmp/folder-libchen/nest", 0, true);
    fs::copy("/tmp/folder-libchen", "/tmp/folder-libchen-copy");
    fs::remove("/tmp/folder-libchen");
    fs::remove("/tmp/folder-libchen_copy");

    // visit
    fs::collect("/", false);
    fs::count("/", false);
    fs::count("/", false, false, false);
    fs::visit("/", [] (const std::string &path) {
    }, false);
#endif
}