# libchen [deprecated]

[![Build Status](https://img.shields.io/travis/chensoft/libchen.svg?logo=travis&label=iOS%20|%20Android%20|%20macOS%20|%20Linux%20|%20Windows)](https://travis-ci.org/chensoft/libchen)
[![Coverage Status](https://img.shields.io/codecov/c/github/chensoft/libchen.svg)](https://codecov.io/gh/chensoft/libchen)
[![release](https://img.shields.io/github/release/chensoft/libchen.svg?colorB=ff69b4)](https://github.com/chensoft/libchen/releases/latest)
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/chensoft/libchen/master/LICENSE)

libchen is a general purpose C++ toolkit, it contains many useful and handy methods for creating complex software. It's cross-platform and released under the MIT License.

## Deprecated

For string format, I prefer [fmtlib/fmt](https://github.com/fmtlib/fmt).

For json parser, I prefer [nlohmann/json](https://github.com/nlohmann/json).

For filesystem, please use [chensoft/libfs](https://github.com/chensoft/libfs).

The library will stop maintenance from now on.

## Platforms

* iOS (Xcode 8+)
* Android (NDK r10e+)
* macOS (Xcode 8+)
* Linux (GCC 5+)
* Windows (VS 2015+)

## Features

* base - basic part
    * str: string format, print, equality test, prefix & suffix, join, replace, trim and more
    * utf8: check utf-8 string character count, retrieve sub string, check bom
    * any: provide a wrapper to hold any type
    * num: float equality test, integer swap, number to string, random number generator
    * map: check key existence, retrieve all keys, all values, flip map's key and value
    * vector: make unique, combine, intersect and difference
    * regex: simple wrapper for regex match, replace and group
* data - data parse
	* json: json file & string parser
	* ini: ini file & string parser
* mt - multithreading
	* semaphore: C++11 semaphore support
	* threadpool: threadpool support, use std::future
* sys - system operation
	* fs: dirname, basename, realpath, file exist and many filesystem related methods
	* sys: uuid generator
	* proc: create daemon easily, get process pid and kill process
* time - time and date
	* time: simple timing class
	* date: thread-safe gmt & local time
* tool - misc tools
	* log: simple logging system
	* cmd: modern command line parser

## Compile

### Method 1 - Via Source

You can directly add the source code to your project, the `include` folder contains the headers, the `src` folder contains the source code. Don't add `test` folder, they are the test code.

Please note that the `src/plat` folder contains some platform dependent code, if you use Unix-like os, just add the `src/plat/unix` folder, exclude others. If you use Windows, add the `src/plat/win` folder only.

### Method 2 - Via Library

If you want to compile it to a library, the easiest way is using CMake:

```
git clone https://github.com/chensoft/libchen.git
cd libchen
mkdir build
cd build

cmake ..
make
```

If no errors, a static library named `libchen.a` is generated in the `build` folder.

The default static library has debug info, if you want to build a release library, simply change the build type and compile again:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## License

libchen is released under the MIT License. See the LICENSE file for more information.

## Contact

You can contact me by email: admin@chensoft.com.