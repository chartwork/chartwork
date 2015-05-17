# Build the Windows Installer

This script builds 32- and 64-bit installers for Windows.
The installers copy the plugin-in into Qt Creator and the dlls, libs, and includes into the Qt installation.
In this way, users can use the widgets in Qt Designer as well as compile applications with them.

## Prerequisites

Building the library requires Visual Studio 2013 (12.0) or newer.

The install script requires Python 3.

To build the standalone installer, you will need additional static Qt builds.
These can be built from sources as explained in the [http://qt-project.org/doc/qt-5/windows-building.html|Qt5 building guide] using a configuration like this:
configure -static -nomake tests -nomake examples -prefix C:\Qt-[architecture]-static\Qt5.3.2\5.3\ -opensource -no-opengl

Qt 64-bit currently (5.4.1) ships with a 32-bit version of QtCreator.
You, hence, have to build the x86 version first in order to create the 64-bit installer.

## Build Steps

1. Create a settings.py by copying the settings.py.example and adjusting the paths and make tool to your installation.
1. Open the Visual Studio Command Prompt matching the required architecture.
1. Run python package.py [architecture] to build the installer (x86 or amd64).
