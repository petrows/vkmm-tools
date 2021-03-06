#!/bin/bash

export PATH="$PATH:/usr/bin:/usr/sbin:/sbin:/bin"

set -e

TOOL_CMAKE="cmake.exe"
TOOL_MSBUILD="MSBuild.exe"

rm -rf build
mkdir -p build
mkdir -p build/out
cd build

$TOOL_CMAKE .. -G "Visual Studio 11" -T "v110_xp" -DCMAKE_INSTALL_PREFIX="$(pwd)"

$TOOL_MSBUILD /p:Configuration=Release /m:2 /p:BuildInParallel=true INSTALL.vcxproj

cd out
for exe in $(ls *.exe); do
	echo "EXE: $exe"
	windeployqt.exe "$exe"
done

