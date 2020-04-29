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
- `make ALG`, where *ALG* is one of *KruskalSimple*, *KruskalUnionFind*,
    *KruskalUnionFindCompressed*, *PrimBinaryHeap*, *PrimKHeap* to 
    compile given algorithm sources.
- `make testall`, to run tests on our algorithms.
- `make testall_explicit`, to run tests with verbose output on our algoritms.
- `make report`, to build the report pdf.

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
You can look at `test.sh`, `testall.sh` and `time.sh`. Note that the Linux
scripts are not always up-to-date.

## Project Structure

The project is a Visual Studio solution, so you can open it with VS. Each
algorithm is a project of the solution, while the shared data structure and
algorithms are stored in the *Shared* folder.

The project comes with some extra folders:
- *benchmark*, that contains benchmark results for the algorithms;
- *dataset*, that contains benchmakr input data, i.e. about 70 random graph 
    from 10 to 100k nodes;
- *report*, that contains the sources for our report about the project;
- *test*, that contains tests to verify algorithms correctness. 

## Authors

**Bryan Lucchetta**
- GitHub: [@1-coder](https://github.com/1-coder)

**Luca Parolari**
- GitHub: [@lparolari](https://github.com/lparolari)

**Alberto Schiabel**
- GitHub: [@jkomyno](https://github.com/jkomyno)

## License

This project is MIT licensed. See [LICENSE](LICENSE) file.
