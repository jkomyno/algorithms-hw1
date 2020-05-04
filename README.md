# Algorithms-HW1

[![Build Status](https://travis-ci.com/jkomyno/algorithms-hw1.svg?token=VSm1u6swXqyzsdGeq7Kp&branch=master)](https://travis-ci.com/jkomyno/algorithms-hw1)

> Minumum Spanning Tree with Kruskal and Prim algorithms.

An hands-on experience about algorithms over graph and graph theory
with the focus on efficiency and, obviously, correctness.

## Usage

The repositores comes with some utility commands that allows you to
compile, test and bench owr algorithms. We use *make* to automatize
this process.

**Available commands**

- `make all`, to compile all the algorithm sources in this project.
- `make ALG`, where *ALG* is one of *KruskalNaive*, *KruskalUnionFind*,
    *KruskalUnionFindCompressed*, *PrimBinaryHeap*, *PrimKHeap* to
    compile given algorithm sources.
- `make testall`, to run tests on our algorithms.
- `make testall_explicit`, to run tests with verbose output on our algoritms.

Within the Makefile we provieded some variables to modify our pipeline.
In particular you can use your own compiler rewriting the `CXX` flag. Other
variables can be modified, for example `CXXFLAGS`, `OUT_DIR` and `EXT`.

Example
```
make CXX="g++" CXXFLAGS="-O3 -std=c++17 -I Shared" OUT_DIR="build" EXT="exe" all
```

**Scripts**

If you are a Windows user you can look at `test.ps1`, `testall.ps1` and
`time.ps1` scripts in order to run tests and bench algorithms.

If you are a Linux user, we have created a porting of the above scripts.
You can look at `test.sh`, `testall.sh` and `time.sh`. Note that these Linux scripts have less features than their Windows counterpart.

## Project Structure

The project is structured as a unique Visual Studio solution containing multiple subprojects, one for every implemented algorithm.
The code for each project is stored in a folder with the same name of the related algorithm.
These projects are:

* [KruskalNaive](./KruskalNaive): Kruskal MST with simple DFS cycle detection;
* [KruskalUnionFind](./KruskalUnionFind): Kruskal MST implemented with Disjoint-Set (Union-Find) data structure, with union-by-size policy;
* [KruskalUnionFindCompressed](./KruskalUnionFindCompressed): Kruskal MST implemented with Disjoint-Set (Union-Find) data structure, with union-by-rank policy and path-compression;
* [PrimBinaryHeap](./PrimBinaryHeap): Prim MST with a Priority Queue based on a Binary Heap;
* [PrimKHeap](./PrimKHeap): Prim MST with a Priority Queue based on a K-ary Heap.

The shared data structures and utils are stored in the *Shared* folder.

The project comes with some extra folders:
* **benchmark**: it contains CSV benchmarks of the algorithm as well as the script used to analyze them ([analysis.py](./benchmark/analysis.py));
* **datasets**: it contains the input data for the graphs given by our professor, i.e. 68 random connected, weighted and non-directed graphs up to 100K nodes and ~130K edges;
* **test**: it contains 68 test graphs and their exact MST value, used test our algorithms' correctness.

## Related Projects

Some of the data-structures created for this project are general enough that we were able to copy their source to
separate, indipendent repositories, each with its own documentation and unit tests.
These repositories are:

* [**priority-queue**](https://github.com/jkomyno/priority-queue) contains the sources [Heap.h](./Shared/Heap.h),
  [BinaryHeap.h](./Shared/BinaryHeap.h), [KHeap.h](./Shared/KHeap.h), [PriorityQueue.h](./Shared/PriorityQueue.h), as
  well as unit tests and example usage documentation.
* [**disjoint-set**](https://github.com/jkomyno/disjoint-set) contains the sources [DisjointSetBase.h](./Shared/DisjointSetBase.h),
  [DisjointSet.h](./Shared/DisjointSet.h), [DisjointSetCompressed.h](./Shared/DisjointSetCompressed.h), as
  well as unit tests and example usage documentation.

## Authors

**Bryan Lucchetta**
- GitHub: [@1-coder](https://github.com/1-coder)

**Luca Parolari**
- GitHub: [@lparolari](https://github.com/lparolari)

**Alberto Schiabel**
- GitHub: [@jkomyno](https://github.com/jkomyno)

## License

This project is MIT licensed. See [LICENSE](LICENSE) file.
