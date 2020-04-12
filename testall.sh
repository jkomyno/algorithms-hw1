# Usage: ./testall.sh

test_script="test.sh"
programs="KruskalUnionFind.out KruskalUnionFindCompressed.out PrimBinaryHeap.out"

# TODO: KruskalSimple.out is excluded from programs list due to its execution time.

failed_no=0

for program in ${programs}; do
  	printf "Testing ${program} ...  "
	p=$(./${test_script} ${program})
	if [ p ]; then
		echo "OK"
	else
		failed_no=$((failed_no+1))
		echo "FAILED"
	fi
done

if [ ${failed_no} -eq 0 ]; then
	echo "All test passed."
	exit 0
else
	echo "There are failures."
fi
