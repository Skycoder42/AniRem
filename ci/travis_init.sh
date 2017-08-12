#!/bin/bash
set -e

currDir=$(dirname $0)

echo '!android: CONFIG += no_mobile' >> .qmake.conf

# install 7z
if [[ $TRAVIS_OS_NAME == "linux" ]]; then
	sudo apt-get -qq update
	sudo apt-get -qq install --no-install-recommends p7zip-full
fi

if [[ $TRAVIS_OS_NAME == "osx" ]]; then
	brew install p7zip
fi

# unpack data
mkdir ../__private
7z x "-p$PROXER_DATA" -o../__private "./ci/proxer.data"

# move data into docker
if [[ $PLATFORM == "gcc_64" ]]; then
	mv ../__private ./ci/__private
	# prepend to build file to run inside docker
	mv ./qtmodules-travis/ci/linux/build-all.sh ./qtmodules-travis/ci/linux/build-all-orig.sh
	mv $currDir/prepare.sh ./qtmodules-travis/ci/linux/build-all.sh
fi

# osx fixup
if [[ $TRAVIS_OS_NAME == "osx" ]]; then
	echo "CONFIG += debug" >> .qmake.conf
	echo "DEFINES += QT_NO_DEBUG" >> .qmake.conf
fi

# rebuild qrestbuilder for ubuntu 14 (TO OLD!!!)
if [[ $PLATFORM == "android_"* ]]; then
	sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
	sudo apt-get -qq update
	sudo apt-get -qq install --no-install-recommends g++-6

	pushd $(mktemp -d)
	
	git clone https://github.com/Skycoder42/QtRestClient.git

	echo 'QMAKE_CXX=g++-6' >> QtRestClient/.qmake.conf
	echo 'SUBDIRS = ' >> QtRestClient/examples/examples.pro
	echo 'SUBDIRS = ' >> QtRestClient/src/src.pro
	echo 'SUBDIRS = ' >> QtRestClient/tests/tests.pro

	mkdir build
	cd build
	/opt/qt/$QT_VER/gcc_64/bin/qmake -r ../QtRestClient/
	cd tools/qrestbuilder
	make
	sudo make uninstall
	sudo make install
	
	popd
fi
