# Build the Mac Installer

This script builds an installer for Mac OS/X.
The installer copies the plugin-in into Qt Creator and the dylibs, libs, and includes into the Qt installation.
In this way, users can use the widgets in Qt Designer as well as compile applications with them.

## Prerequisites

Building the library requires gcc 4.8 or newer and Qt 5.4 or newer.
Clang is currently not supported.

The install script requires Python 3 and cmake.

## Build Steps

1. Create a settings.py by copying settings.py.example and adjusting the paths to your installation.
1. Run python package.py mac to build the installer.
