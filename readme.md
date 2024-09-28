PBSketch
============

Repository structure
--------------------
*  `common/`: some function and clas used by all algorithms like the hash function.
*  `BaseLine/`: the implementation of baseline
*  `PBSketch/`: the implementation of PBSketch
*  `benchmark/`: code of memory test about accuracy,recall,f1,aae,are and throughput test
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