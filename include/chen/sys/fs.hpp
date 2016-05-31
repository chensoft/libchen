/**
 * Created by Jian Chen
 * @since  2016.01.29
 * @author Jian Chen <admin@chensoft.com>
 * @link   http://chensoft.com
 */
#pragma once

#include <functional>
#include <string>
#include <vector>

namespace chen
{
    class fs
    {
    public:
        /**
         * Current system drive directory
         * e.g: "C:\" on Windows, "/" on other os
         */
        static std::string root();

        /**
         * Current home directory
         */
        static std::string home();

        /**
         * Current temp directory
         */
        static std::string temp();

        /**
         * Current working directory
         */
        static std::string current();

        /**
         * On Windows return "C:\", "D:\" and so on
         * On other os return "/"
         */
        static std::vector<std::string> drives();

        /**
         * Separator on this platform
         * @result '/' on Unix or Linux, '\' on Windows
         */
        static char separator();

        /**
         * Expand all symbolic links and remove ".", ".." and redundant separators
         * this function will check the existence of the path, if path is invalid will return empty
         */
        static std::string realpath(const std::string &path);

        /**
         * Absolute path, append to current working directory, without ".", ".." and separators
         * this function didn't check the existence of the path and didn't follow symbolic link
         * e.g: if cwd is /home/staff/Downloads, then path "file.txt" will be appended to cwd
         */
        static std::string absolute(const std::string &path);

        /**
         * Normalize path, remove ".", ".." and redundant separators
         * this function didn't check the existence of the path and didn't follow symbolic link
         * e.g: ./a to a, a/./b to a/b, a///b to a/b
         * e.g: a/.../b to a/...b because the "..." is invalid path characters, it will be ignored
         * e.g: a/../../b to ../b because the path is relative and second ".." can't be removed
         */
        static std::string normalize(const std::string &path);

        /**
         * Directory name of the path, without the trailing slash
         * e.g: /home/staff/Downloads/file.txt, dirname is "/home/staff/Downloads"
         * e.g: /usr/., dirname is "/usr", because "." is represent current directory
         * e.g: /usr/, dirname is "/", not "/usr", because single "/" isn't a effective name
         * e.g: /usr///, dirname is "/", because the trailing slash will be ignored
         * e.g: /, dirname is "/", because it's already the root directory
         * e.g: file.txt, dirname is ".", because it's a relative path
         */
        static std::string dirname(const std::string &path);

        /**
         * Base name of the path
         * e.g: /home/staff/Downloads/file.txt, basename is "file.txt"
         * e.g: file.txt, basename is "file.txt"
         */
        static std::string basename(const std::string &path);

        /**
         * Extension name of the path
         * e.g: /home/staff/Downloads/file.txt, extname is ".txt"
         * @param path full path
         * @param dots max dots count of the extension
         */
        static std::string extname(const std::string &path, std::size_t dots = 1);

    public:
        /**
         * Check if the file or directory exist
         */
        static bool isExist(const std::string &path);

        /**
         * Check if the path is a directory
         */
        static bool isDir(const std::string &path, bool strict = false);

        /**
         * Check if the path is a regular file
         * @param strict false if it will follow the symbolic link
         */
        static bool isFile(const std::string &path, bool strict = false);

        /**
         * Check if the path is a symbolic link
         */
        static bool isLink(const std::string &path);

        /**
         * Check if the path is an absolute path
         */
        static bool isAbsolute(const std::string &path);

        /**
         * Check if the path is a relative path
         */
        static bool isRelative(const std::string &path);

        /**
         * Check if the file or directory is readable
         */
        static bool isReadable(const std::string &path);

        /**
         * Check if the file or directory is writable
         */
        static bool isWritable(const std::string &path);

        /**
         * Check if the file or directory is executable
         */
        static bool isExecutable(const std::string &path);

    public:
        /**
         * Get file or directory's access time
         * @result the unix timestamp
         */
        static time_t atime(const std::string &path);

        /**
         * Get file or directory's modification time
         */
        static time_t mtime(const std::string &path);

        /**
         * Get file or directory's create time
         */
        static time_t ctime(const std::string &path);

        /**
         * Get file size
         */
        static off_t filesize(const std::string &file);

    public:
        /**
         * Set access and modification time of the file, create file and its dir if it's not exist
         * @param file the file to be access or create
         * @param mtime modification time, if zero then use current time
         * @param atime access time, if zero then use mtime
         */
        static bool touch(const std::string &file, time_t mtime = 0, time_t atime = 0);

        /**
         * Create a directory
         * @param mode default mode is rwxr-xr-x
         * @param recursive recursively or not
         */
        static bool create(const std::string &dir, std::uint16_t mode = 0, bool recursive = true);

        /**
         * Rename a file or directory
         */
        static bool rename(const std::string &path_old, const std::string &path_new);

        /**
         * Remove a file or directory
         */
        static bool remove(const std::string &path);

        /**
         * Copy a file or directory
         */
        static bool copy(const std::string &path_old, const std::string &path_new);

    public:
        /**
         * Change current working directory
         */
        static bool change(const std::string &directory);

        /**
         * Visit the directory items use depth-first traversal, exclude '.' and '..'
         * @param recursive (optional) check items recursively
         */
        static void visit(const std::string &directory,
                          std::function<bool (const std::string &path)> callback,
                          bool recursive = true);

        /**
         * Collect all items in the directory, exclude '.' and '..'
         */
        static std::vector<std::string> collect(const std::string &directory,
                                                bool recursive = true);

        /**
         * The files and directories count in the directory
         * @param contain_file calculate the files count
         * @param contain_dir calculate the directories count
         */
        static std::size_t count(const std::string &directory,
                                 bool recursive = true,
                                 bool contain_file = true,
                                 bool contain_dir = true);
    };
}