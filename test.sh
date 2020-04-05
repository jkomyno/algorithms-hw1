#!/bin/bash

# Usage: ./test.sh [program-name] > output.csv
# Execute [program-name] against all the input files in test/ and compares the result
# of the program with the corresponding output file

# Note: this file needs the LF line endings.

program=$1
test_dir=test

test_no=0
failed_no=0
success_no=0

for test_file in ${test_dir}/input_random*.txt; do
	test_no=$((test_no+1))

	input_file="${test_file}"
	output_file=$(echo ${input_file/input/output})

	actual=$(cat ${input_file} | ./${program})
	expected=$(cat ${output_file})

	if [ "${expected}" == "${actual}" ]; then
		success_no=$((success_no+1))
		echo "OK ${input_file}"
	else
		failed_no=$((failed_no+1))
		echo "MST mismatch on input ${input_file}. Got ${actual}, expected ${expected}."
	fi
done

echo ""
echo "Executed ${test_no} tests, ${success_no} succeeded and ${failed_no} failed."
if [ ${failed_no} -eq 0 ]; then
	exit 0
else
	exit 1
fi
