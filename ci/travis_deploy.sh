#!/bin/bash
set -e

if [[ $PLATFORM == "android_"* ]]; then
	mkdir -p ./install
	$HOME/android/sdk/build-tools/*/apksigner sign --ks "$(dirname $0)/anirem.aks" --ks-pass env:AKS_DATA --key-pass env:AKS_DATA --out "./install/anirem-${ANIREM_VER}_${PLATFORM}.apk" "build-$PLATFORM/android-build/build/outputs/apk/android-build-release-unsigned.apk"
else
	cd install
	
	if [ "$TRAVIS_OS_NAME" == "osx" ]; then
		repname=mac_x64
	else
		repname=${TRAVIS_OS_NAME}_x64
	fi
	
	mv repository "$repname"
	tar cJf repository_$TRAVIS_OS_NAME.tar.xz "$repname"
fi
