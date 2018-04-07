#!/bin/bash
set -e

if [[ $PLATFORM == "android_"* ]]; then
	mv install build-$PLATFORM/android-build
	/opt/qt/$QT_VER/$PLATFORM/bin/androiddeployqt --input "build-$PLATFORM/gui/quick/android-libanirem-activity.so-deployment-settings.json" --output "build-$PLATFORM/android-build" --deployment bundled --gradle --no-gdbserver --release
fi
