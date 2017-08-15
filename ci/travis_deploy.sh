#!/bin/bash

if [ "$TRAVIS_OS_NAME" != "android_"* ]; then
	cd install
	
	if [ "$TRAVIS_OS_NAME" == "osx" ]; then
		repname=mac_x64
	else
		repname=${TRAVIS_OS_NAME}_x64
	fi
	
	mv repository "$repname"
	tar cJf repository_$TRAVIS_OS_NAME.tar.xz "$repname"
fi
