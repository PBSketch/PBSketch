PBSketch
============

Repository structure
--------------------
*  `common/`: some function and clas used by all algorithms like the hash function.
*  `BaseLine/`: the implementation of baseline
*  `PBSketch/`: the implementation of PBSketch
*  `benchmark/`: C++ header of some benchmarks about accuracy
*  `ratelimiting/`: the code of rate limiting experiment

Requirements
-------
- cmake
- g++

How to run
-------

```bash
$ git clone https://github.com/PBSketch/PBSketch.git
$ cd ./PBSketch/
$ cmake .
$ make
$ ./PBSketch [datasetPath] [length] -[parameter] to start experiment
    -m: memory test
    -t: throughput test
    -r: Rate Limit test
    -d: Dynamic Buckets amount test
```