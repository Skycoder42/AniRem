#!/bin/bash
set -e

if [[ $PLATFORM == "android_"* ]]; then
	if [[ "$PLATFORM" == "android_armv7" ]]
		PCODE=0
	fi
	if [[ "$PLATFORM" == "android_x86" ]]
		PCODE=1
	fi
	
	sed -i "s/android:versionCode=\"\([[:digit:]]*\)\"/android:versionCode=\"\1%{pcode}\"/g" gui/quick/android/AndroidManifest.xml
	sed -i "s/%{pcode}/$PCODE/g" gui/quick/android/AndroidManifest.xml
	cat gui/quick/android/AndroidManifest.xml

	mv install build-$PLATFORM/android-build
	/opt/qt/$QT_VER/$PLATFORM/bin/androiddeployqt --input "build-$PLATFORM/gui/quick/android-libanirem-activity.so-deployment-settings.json" --output "build-$PLATFORM/android-build" --deployment bundled --gradle --no-gdbserver --release
fi
