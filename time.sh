# Usage: ./time.sh [program-name] > outputs.csv
# create a UTF-16 encoded CSV file that can be easily processed by Python

echo "timing,output"

program=$1
test_dir=test

for test_file in ${test_dir}/input_random*.txt; do
  	test_no=$((test_no+1))

    input_file="${test_file}"

    time $(cat ${input_file} | ./${program}) &>/dev/null
done

