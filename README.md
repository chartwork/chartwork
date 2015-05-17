Chartwork—Chart Widgets for Qt
==================================================

Download Binaries
-----------------

Package binaries for Windows, Mac, and Ubuntu can be downloaded from the [documentation website](http://chartwork.github.io).

Installation, Usage, and Documentation
--------------------------------------

The [documentation website](http://chartwork.github.io) explains how to install the binaries and how to create custom apps with Chartwork.
It also contains a documentation of Chartwork’s public API and a troubleshooting section.

Building Chartwork on Unix Systems
----------------------------------

To build Chartwork from sources, you need the build essentials, cmake, and Qt 5.4 or newer.

As clang is currently not supported, Mac users should use gcc instead (for instance, built with homebrew).

First, get a copy of the Chartwork sources:

```bash
git clone git://github.com/chartwork/chartwork.git
```

Now, create directories for the out-of-source debug and release builds:

```bash
cd chartwork
mkdir -p build/{debug,release}
```

Finally, build the library:

```bash
cd build/debug
cmake ../..
make
cd ../release
cmake ../..
make
```

The compiled in shared object files in `lib/` can be copied to Qt Creator’s designer plug-in directory.
On most Linux systems, this is `/usr/lib/qt/plugins/designer/` and on Mac `${Qt Directory}/Qt Creator.app/Contents/PlugIns/designer`.

Building Packages for Windows, Mac, and Ubuntu
----------------------------------------------

The directory `packaging` contains scripts that semi-automatically build packages for various platforms.

On some platforms, some paths have to be specified manually.
Read the according readme files on how to build.

Contributors
------------

Chartwork was originally written by [Patrick Lühne](http://www.luehne.de) and [Christoph Sterz](http://www.chsterz.de) in 2014 and 2015 and funded by the University of Potsdam.
