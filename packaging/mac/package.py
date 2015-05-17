#!/usr/bin/python3

import sys
import os
import subprocess
import shutil
import glob

if len(sys.argv) < 2 or sys.argv[1] != "mac":
	print("Usage: package.py mac (clean)")
	print()
	print("Specify your build settings in a file called settings.py.")
	print("You can use the settings.py.example as a reference.")
	print("Example usage: package.py mac")
	exit(1)

import settings

buildDirectory = os.path.join(os.getcwd(), "build")
installerBuildDirectory = "build-installer"
installerSourceDirectory = installerBuildDirectory + "-src"
installerDataDirectory = os.path.join(installerSourceDirectory, "data")
installerPlugInDirectory = os.path.join(installerDataDirectory, "plugin")
installerIncludeDirectory = os.path.join(installerDataDirectory, "include")
installerLibDirectory = os.path.join(installerDataDirectory, "lib")

if not settings.environment:
	settings.environment = os.environ.copy()

def clean():
	if os.path.exists(buildDirectory):
		shutil.rmtree(buildDirectory)

	if os.path.exists(installerSourceDirectory):
		shutil.rmtree(installerSourceDirectory)

	if os.path.exists(installerBuildDirectory):
		shutil.rmtree(installerBuildDirectory)

def copy(src, dest, ignore=None):
	if os.path.isdir(src):
		if not os.path.isdir(dest):
			os.makedirs(dest)
		files = os.listdir(src)
		if ignore is not None:
			ignored = ignore(src, files)
		else:
			ignored = set()
		for f in files:
			if f not in ignored:
				copy(os.path.join(src, f), os.path.join(dest, f), ignore)
	else:
		shutil.copy2(src, dest)

def buildLibrary(buildType):
	buildTypeDirectory = os.path.join(buildDirectory, buildType)

	if not os.path.exists(buildTypeDirectory):
		os.makedirs(buildTypeDirectory)

	subprocess.call(["cmake",
					 "-DCMAKE_BUILD_TYPE=" + buildType.upper(),
					 "-DCMAKE_PREFIX_PATH=" + settings.cmakePathPrefixes["dynamic"],
					 "-DCMAKE_CXX_FLAGS=-static-libgcc -static-libstdc++",
					 "../../../.."],
					cwd=buildTypeDirectory, env=settings.environment)
	subprocess.call(settings.make, cwd=buildTypeDirectory, env=settings.environment)

def copyData():
	copy("../../include", installerIncludeDirectory)

	if not os.path.exists(installerPlugInDirectory):
		os.makedirs(installerPlugInDirectory)

	for file in glob.glob(buildDirectory + "/release/lib/*.dylib"):
		copy(file, installerPlugInDirectory)

	if not os.path.exists(installerLibDirectory):
		os.makedirs(installerLibDirectory)

	for file in glob.glob(buildDirectory + "/release/lib/*.dylib") + glob.glob(buildDirectory + "/debug/lib/*.dylib"):
		copy(file, installerLibDirectory)

def strip(filename):
	subprocess.call(["strip", "-u", "-r", "-arch", "all", filename], env=settings.environment)

def installNameToolPlugIn(component, filename):
	subprocess.call(["install_name_tool",
					 "-change",
					 settings.qtDir + "/lib/" + component + ".framework/Versions/5/" + component,
					 "@executable_path/../Frameworks/" + component + ".framework/Versions/5/" + component, filename], env=settings.environment)

def installNameToolLibrary(component, filename):
	subprocess.call(["install_name_tool",
					 "-change",
					 settings.qtDir + "/lib/" + component + ".framework/Versions/5/" + component,
					 "QTDIR/lib/" + component + ".framework/Versions/5/" + component, filename], env=settings.environment)

def installNamesPlugin():
	for file in glob.glob(installerPlugInDirectory + "/*.dylib"):
		strip(file)
		installNameToolPlugIn("QtDesigner", file)
		installNameToolPlugIn("QtWidgets", file)
		installNameToolPlugIn("QtGui", file)
		installNameToolPlugIn("QtCore", file)
		installNameToolPlugIn("QtXml", file)

def installNamesLibraries():
	for file in glob.glob(installerLibDirectory + "/*.dylib"):
		strip(file)
		installNameToolLibrary("QtDesigner", file)
		installNameToolLibrary("QtWidgets", file)
		installNameToolLibrary("QtGui", file)
		installNameToolLibrary("QtCore", file)
		installNameToolLibrary("QtXml", file)

def copyInstallerSources():
	copy("installer", installerSourceDirectory)

def createResourcesFile():
	resourceFile = open(installerSourceDirectory + "/files.qrc", "w")
	resourceFile.write("<RCC>\n")
	resourceFile.write("    <qresource prefix=\"/files\">\n")

	for root, dirs, files in os.walk(installerDataDirectory):
		path = root.split("/")
		for file in files:
			resourceFile.write("        <file>" + os.path.join(os.path.relpath(root, installerSourceDirectory), file) + "</file>\n")

	resourceFile.write("    </qresource>\n")
	resourceFile.write("</RCC>\n")

def buildInstaller():
	if not os.path.exists(installerBuildDirectory):
		os.makedirs(installerBuildDirectory)

	subprocess.call(["cmake",
					 "-DCMAKE_BUILD_TYPE=Release",
					 "-DCMAKE_PREFIX_PATH=" + settings.cmakePathPrefixes["dynamic"],
					 "../" + installerSourceDirectory],
					cwd=installerBuildDirectory, env=settings.environment)
	subprocess.call(settings.make, cwd=installerBuildDirectory, env=settings.environment)

def deployQt():
	subprocess.call([os.path.join(settings.qtDir, "bin/macdeployqt"), "installer.app", "-executable=installer.app/Contents/MacOS/installer", "-dmg"], cwd=installerBuildDirectory, env=settings.environment)

def copyExecutable():
	copy(installerBuildDirectory + "/installer.dmg", "installer.dmg")

if len(sys.argv) > 2 and sys.argv[2] == "clean":
	clean()
	exit(0)

buildLibrary("debug")
buildLibrary("release")
copyData()
installNamesPlugin()
installNamesLibraries()
copyInstallerSources()
createResourcesFile()
buildInstaller()
deployQt()
copyExecutable()
