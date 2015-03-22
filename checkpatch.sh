#!/bin/bash
if [[ -z "$1" ]]; then
	echo "Would you mind providing an input file, please?"
	exit 1
fi

if [[ "$1" == "-w" ]]; then
	#echo "if with $2"
	/home/rounak/Dropbox/Storage/checkpatch.pl --no-tree --file --terse "$2"
else
	#echo "else with $1"
	/home/rounak/Dropbox/Storage/checkpatch.pl --no-tree --file --terse "$1" | awk '/ERROR/'
fi
