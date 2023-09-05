# Tectiform Foundation Libraries

## Background

Over the course of many years of software development at different companies, I noticed a recurring theme in
software development. Companies often need low-level, cross-platform, utility libraries that provide basic
software functionality to develop applications.  Some companies choose to use vendor provided libraries, or make
use of open source libraries such as Boost.  Boost is a good set of libraries, but I have often found that it did
not quite provide the right set of interfaces that I often needed.  I decided to build my own set of open source
libraries to meet my needs.  With an open library, I can develop code once and re-use it for any organization that
will adopt it.  I chose a BSD style license to allow companies the flexibility of modifying the code without needing
to contribute changes back to the original project.  This license makes the software palatable in organizations that
might not have otherwise used the software.

I started working on the Tectiform Foundation Libraries in 2015 to overcome the problems of developing low level,
proprietary libraries that could not follow me from job to job.  In no way should these libraries be considered
reliable code at the level of Boost (after all I am only one developer), but the do provide low level functionality
that can often be difficult and time-consuming to implement if you have the need for such functionality.

While I intended the foundation libraries to have cross-platform support, at the moment, the libraries only work
for macOS and Linux platforms.  I have had no time or resources to support development for Window.  You may find
places in the library that have Windows support roughed in, but no code modules have decent support for Windows.

In the process of developing the library, I have found some pieces of functionality available in the C++ STL.
As such, some sub-modules such we no longer need some of sub-modules such as the Thread module.  In other cases,
I have found that the STL provides very nice low-level support, but I needed a better interface on top of the STL
functionality.

## Prerequisites

### Conan

The Conan developers recently released Conan 2.0.0.  This new version has incompatibilities with the CMake infrastructure
used by the Tectiform Foundation Libraries build system.  Until that system can support Conan 2.0.0 we will use
Conan version 1.59.0.

### macOS

Download and install Xcode from the Apple App Store.  Be sure to start Xcode after installation to allow Xcode to
install any platform specific packages.  Xcode provides C/C++ compilers and platform SDKs that you will need to
successfully build and use the Tectiform Foundation Libraries.

Download and install the [Homebrew](https://brew.sh/) package.  Follow the instructions on the website to download
and install the Homebrew system on you machine.  After installation, install the following packages:

* Python 3 - `brew install python3`
* Conan - `pip3 install conan==1.59.0`
* Cmake - `brew install cmake`
* Clang-format - `brew install clang-format`
* Ninja - `brew install ninja`

### Linux

I have developed the Linux code for Tectiform Foundation Libraries using Ubuntu derivatives.  You can build the
libraries on other versions, but the following instructions assume an Ubuntu platform or an Ubuntu derivative.
