import sys
import os
import subprocess
import shutil
import glob

if len(sys.argv) < 2:
	print("Usage: package.py [architecture] (clean)")
	print()
	print("Specify your build settings in a file called settings.py.")
	print("You can use the settings.py.example as a reference.")
	print("Example usage: package.py x86")
	exit(1)

import settings

architecture = sys.argv[1]

buildDirectory = "build-" + architecture
buildX86Directory = "build-x86"
installerBuildDirectory = "build-" + architecture + "-installer"
installerSourceDirectory = installerBuildDirectory + "-src"
installerDataDirectory = os.path.join(installerSourceDirectory, "data")
installerBinDirectory = os.path.join(installerDataDirectory, "bin")
installerPlugInDirectory = os.path.join(installerDataDirectory, "plugin")
installerIncludeDirectory = os.path.join(installerDataDirectory, "include")
installerLibDirectory = os.path.join(installerDataDirectory, "lib")

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
		"-G" + settings.cmakeGenerator,
		"-DCMAKE_BUILD_TYPE=" + buildType.upper(),
		"-DCMAKE_PREFIX_PATH=" + settings.cmakePathPrefixes[architecture]["dynamic"],
		"../../../../"], shell=True, cwd=buildTypeDirectory)
	subprocess.call(settings.make, shell=True, cwd=buildTypeDirectory)

def copyData():
	copy("../../include", installerIncludeDirectory)

	if not os.path.exists(installerPlugInDirectory):
		os.makedirs(installerPlugInDirectory)

	for file in glob.glob(buildX86Directory + "/release/bin/*.dll"):
		copy(file, installerPlugInDirectory)

	if not os.path.exists(installerBinDirectory):
		os.makedirs(installerBinDirectory)

	for file in glob.glob(buildDirectory + "/release/bin/*.dll") + glob.glob(buildDirectory + "/debug/bin/*.dll") + glob.glob(buildDirectory + "/debug/bin/*.ilk") + glob.glob(buildDirectory + "/debug/bin/*.pdb"):
		copy(file, installerBinDirectory)

	if not os.path.exists(installerLibDirectory):
		os.makedirs(installerLibDirectory)

	for file in glob.glob(buildDirectory + "/release/lib/*.lib") + glob.glob(buildDirectory + "/release/lib/*.exp") + glob.glob(buildDirectory + "/debug/lib/*.lib") + glob.glob(buildDirectory + "/debug/lib/*.exp"):
		copy(file, installerLibDirectory)

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
		"-G" + settings.cmakeGenerator,
		"-DCMAKE_BUILD_TYPE=Release",
		"-DCMAKE_PREFIX_PATH=" + settings.cmakePathPrefixes[architecture]["static"],
		"../" + installerSourceDirectory], shell=True, cwd=installerBuildDirectory)
	subprocess.call(settings.make, shell=True, cwd=installerBuildDirectory)

def copyExecutable():
	copy(installerBuildDirectory + "/bin/installer.exe", "installer-" + architecture + ".exe")

if len(sys.argv) > 2 and sys.argv[2] == "clean":
	clean()
	exit(0)

# The 64-bit version requires a 32-bit build of the plug-in, as Qt ships with 32-bit Qt Creator
if architecture == "amd64" and not os.path.exists("build-x86"):
	print("Please build the x86 version first, as Qt ships with 32-bit Qt Creator even in the 64-bit version")
	exit(1)

buildLibrary("debug")
buildLibrary("release")
copyData()
copyInstallerSources()
createResourcesFile()
buildInstaller()
copyExecutable()
