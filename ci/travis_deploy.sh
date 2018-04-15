#!/bin/bash
set -e

if [[ $PLATFORM == "android_"* ]]; then
	# install android deps for signing
	sudo apt-get -qq update
	sudo apt-get -qq install --no-install-recommends openjdk-8-jdk unzip

	# android skd
	curl -Lo /tmp/android-sdk.zip https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip
	mkdir $HOME/android
	unzip -qq /tmp/android-sdk.zip -d $HOME/android/sdk/
	rm -f /tmp/android-sdk.zip
	echo y | $HOME/android/sdk/tools/bin/sdkmanager --update > /dev/null
	echo y | $HOME/android/sdk/tools/bin/sdkmanager "build-tools;27.0.3" > /dev/null

	# sign
	mkdir -p ./install
	$(find "$HOME/android" -name apksigner) sign --ks "$(dirname $0)/anirem.aks" --ks-pass env:AKS_DATA --key-pass env:AKS_DATA --out "./install/anirem-${ANIREM_VER}_${PLATFORM}.apk" "build-$PLATFORM/android-build/build/outputs/apk/android-build-release-unsigned.apk"
fi
