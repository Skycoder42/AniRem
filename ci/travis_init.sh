#!/bin/bash
set -e

currDir=$(dirname $0)

echo '!android: CONFIG += no_quick' >> .qmake.conf

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
	sudo apt-get -qq install --no-install-recommends xz-utils
	pushd gui/quick/openssl
	./get_openssl.sh
	popd
fi
