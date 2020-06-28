#!/bin/sh

for TEST in "$@"
do
	if [[ -x "${TEST}" ]]
	then
		echo "Running ${TEST} test..."
		sh -c ./${TEST}
		if [ $? -ne 0 ]
		then
			exit 1
		fi
	fi
	if [[ -x "${TEST}.py" ]]
	then
		sh -c ./${TEST}.py
		if [ $? -ne 0 ]
		then
			exit 1
		fi
	fi
done
