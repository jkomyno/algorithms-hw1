CXX=g++-9
CXXFLAGS=-O3 -Wall -Wextra -std=c++17 -I Shared
MAINFILE=main.cpp

KRUSKAL_SIMPLE=KruskalSimple
KRUSKAL_UNION_FIND=KruskalUnionFind
PRIM_BINARY_HEAP=PrimBinaryHeap

all: ${KRUSKAL_SIMPLE} ${KRUSKAL_UNION_FIND} ${PRIM_BINARY_HEAP}

${KRUSKAL_SIMPLE}:
	${CXX} ${CXXFLAGS} ${KRUSKAL_SIMPLE}/${MAINFILE} -o ${KRUSKAL_SIMPLE}.out

${KRUSKAL_UNION_FIND}:
	${CXX} ${CXXFLAGS} ${KRUSKAL_UNION_FIND}/${MAINFILE} -o ${KRUSKAL_UNION_FIND}.out

${PRIM_BINARY_HEAP}:
	${CXX} ${CXXFLAGS} ${PRIM_BINARY_HEAP}/${MAINFILE} -o ${PRIM_BINARY_HEAP}.out

testall:
	./testall.sh

testall_explicit:
	./test.sh ${KRUSKAL_UNION_FIND}.out
	./test.sh ${PRIM_BINARY_HEAP}.out
# TODO: Test KruskalSimple excluded due to its runtime.
# ./test.sh ${KRUSKAL_SIMPLE}.out

.PHONY: all ${KRUSKAL_SIMPLE} ${KRUSKAL_UNION_FIND} ${PRIM_BINARY_HEAP}
