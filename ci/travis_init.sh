#!/bin/bash
set -e

currDir=$(dirname $0)

echo 'CONFIG += create_installer' >> install.pri

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
7z x "-p$PROXER_DATA" -o. "./ci/proxer.data" > /dev/null

# install openssl for android
if [[ $PLATFORM == "android_"* ]]; then
	pushd gui/quick/openssl
	./get_openssl.sh
	popd
	
	# append droid build script
	echo "$currDir/travis_postbuild.sh" >> qtmodules-travis/ci/linux/build-docker.sh
fi
