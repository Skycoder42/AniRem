#!/bin/bash
set -e

path=$(dirname $0)/android.xks

if [[ $PLATFORM == "android_"* ]]; then
	mv install build-$PLATFORM/android-build
	/opt/qt/$QT_VER/$PLATFORM/bin/androiddeployqt --input "build-$PLATFORM/Mobile/android-libAniRem.so-deployment-settings.json" --output "build-$PLATFORM/android-build" --deployment bundled --gradle --no-gdbserver --release --sign "$path" anirem --storepass "$XKS_DATA"
fi
