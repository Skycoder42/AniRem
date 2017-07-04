#!/bin/sh

for file in $(find . -name "qpm.json"); do
	qpmdir=$(dirname $file)
	if [[ "$qpmdir" != *"vendor"* ]]; then
		olddir=$(pwd)
		cd $qpmdir
		qpm install
		cd $olddir
	fi	
done
