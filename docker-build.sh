#!/bin/sh
# $1 local install directory

image=skycoder42/qt-build
echo Building with image $image
docker run --rm -e "QPM_PATH=Core:Desktop:Mobile" --name docker-qt-build -v "$1:/tmp/qt-build/inst" -v "/home/sky/Programming/QtProjects/__private:/root/__private" -e "BUILD_GIT_SRC=https://github.com/Skycoder42/SeasonProxer" $image