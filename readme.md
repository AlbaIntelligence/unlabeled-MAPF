Unlabeled MAPF
===
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENCE.txt)

A simulator and visualizer of Unlabeled Multi-Agent Path Finding (MAPF), used in the manuscript.
It is written in C++(17) with [CMake](https://cmake.org/) build and tested on MacOS 10.15.
The repository uses [Google Test](https://github.com/google/googletest).
The visualizer uses [openFrameworks](https://openframeworks.cc).
It is possible to use [GUROBI9.1](https://www.gurobi.com/) as an ILP solver to solve the maximum flow problems.

The implementations include: the makespan optimal algorithm [1] and TSWAP (sub-optimal, complete).

## Demo
![demo in a small field, flocking-like](/material/arena_100agents.gif)

![demo in a large field](/material/lak303d_300agents.gif)

## Building

```
git clone https://github.com/google/googletest.git third_party/googletest
mkdir build
cd build
cmake ..
make
```

## Usage
TSWAP (sub-optimal complete)
```sh
./app -i ../instances/sample.txt -s TSWAP -o result.txt -v
```

FlowNetwork (makespan optimal, the result is saved in `result.txt`)
```sh
./app -i ../instances/random-32-32-20_70agents_1.txt -s FlowNetwork -v
```

You can find details and explanations for all parameters with:
```sh
./app --help
```

Please see `instances/sample.txt` for parameters of instances, e.g., filed, number of agents, time limit, etc.

### Output File

This is an example output of `../instances/sample.txt`.
Note that `(x, y)` denotes location.
`(0, 0)` is the left-top point.
`(x, 0)` is the location at `x`-th column and 1st row.
```
instance=../instances/sample.txt
agents=100
map_file=arena.map
solver=TSWAP
solved=1
soc=1864
makespan=30
comp_time=91
internal_info=
elapsed_assignment:82
elapsed_path_planning:9
estimated_soc:1602
estimated_makespan:29
starts=(32,21),(18,22),(29,19),(26,24),[...]
goals=(39,4),(23,18),(46,2),(26,26),[...]
solution=
0:(32,21),(18,22),(29,19),(26,24),[...]
1:(32,20),(18,21),(30,19),(26,23),[...]
[...]
```

## Installation of GUROBI
As an option, you can use an ILP solver (GUROBI9.1) in the optimal solver.
The code of GUROBI is available free for academic use.
Install the code following instructions on the [official site](https://www.gurobi.com/documentation/9.1/quickstart_mac/software_installation_guid.html).
Don't forget to setup an academic license.

After the installation, make an alias of `GUROBI_HOME`.
My environment is as follows.
```
> echo ${GUROBI_HOME}
/Library/gurobi911/mac64
```

You can test the ILP solver via `-g` option.
```
./app -i ../instances/random-32-32-20_70agents_1.txt -s FlowNetwork -v -g
```

Note: Of course, it is significantly slower than the flow-based algorithm (i.e., Ford-Fulkerson).

## Visualizer
### Build
Unfortunately, [the latest distribution of openFrameworks](https://github.com/openframeworks/openFrameworks/commit/fe591d17e95218569cc2426d1d8f4f646f75fa00) seems to include bugs,
so you have to edit one file according to the following instructions.

Let's first clone the repo.
```sh
git clone https://github.com/openframeworks/openFrameworks.git third_party/openFrameworks
```

Edit two places in `./third_party/openFrameworks/scripts/download_libs.sh` as follows to enable to download [the old libs](http://ci.openframeworks.cc/libs/0.11.0/).

[Before, Line 175](https://github.com/openframeworks/openFrameworks/blob/patch-release/scripts/dev/download_libs.sh#L175)

```sh
elif [ "$PLATFORM" == "vs2015" ] || [ "$PLATFORM" == "vs2017" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_4.zip"
elif [ "$ARCH" == "" ] && [[ "$PLATFORM" == "osx" || "$PLATFORM" == "ios" || "$PLATFORM" == "tvos" ]]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}1.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}2.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}3.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}4.tar.bz2"
```

After

```sh
elif [ "$PLATFORM" == "vs2015" ] || [ "$PLATFORM" == "vs2017" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_4.zip"
elif [ "$ARCH" == "" ] && [[ "$PLATFORM" == "osx" || "$PLATFORM" == "ios" || "$PLATFORM" == "tvos" ]] && [ "$VER" != "" ]; then
    PKGS="openFrameworksLibs_master_${PLATFORM}1.tar.bz2 \
          openFrameworksLibs_master_${PLATFORM}2.tar.bz2 \
          openFrameworksLibs_master_${PLATFORM}3.tar.bz2 \
          openFrameworksLibs_master_${PLATFORM}4.tar.bz2"
elif [ "$ARCH" == "" ] && [[ "$PLATFORM" == "osx" || "$PLATFORM" == "ios" || "$PLATFORM" == "tvos" ]]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}1.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}2.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}3.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}4.tar.bz2"
```

[Before, Line 193](https://github.com/openframeworks/openFrameworks/blob/patch-release/scripts/dev/download_libs.sh#L193)
```sh
for PKG in $PKGS; do
    download $PKG
done
```

After
```sh
for PKG in $PKGS; do
    if [ "$VER" != "" ]; then
        download $VER/$PKG
    else
        download $PKG
    fi
done
```

Next, build as follows.
It takes around 10 minutes.
```sh
sh ./third_party/openFrameworks/scripts/osx/download_libs.sh -v 0.11.0
cd visualizer/
make build
cd ..
chmod +x ./visualize.sh
```

### Usage
```sh
cd build
../visualize.sh result.txt
```

You can manipulate it via your keyboard. See printed info.

## Licence
This software is released under the MIT License, see [LICENCE.txt](LICENCE.txt).

## Notes
- `NaiveTSWAP` is a solver using the pseudo-code in the paper without modifications.
  `TSWAP` uses a priority queue to achieve efficient agents' moves.
- Maps in `maps/` are from [MAPF benchmarks](https://movingai.com/benchmarks/mapf.html).
  When you add a new map, please place it in the `maps/` directory.
- The font in `visualizer/bin/data` is from [Google Fonts](https://fonts.google.com/).
- Scripts for the experiments are in `exp_scripts/`.
- `tests/` include test scripts.
- The implementation of ECBS-TA [2] can be obtained [Wolfgang's excellent repository](https://github.com/whoenig/libMultiRobotPlanning).

## Author
Anonymous Authors

## Reference
1. Yu, J., & LaValle, S. M. (2013).
   Multi-agent path planning and network flow.
   In Algorithmic foundations of robotics X (pp. 157-173). Springer, Berlin, Heidelberg.
2. Hönig, W., Kiesel, S., Tinka, A., Durham, J., & Ayanian, N. (2018).
   Conflict-based search with optimal task assignment.
   In Proceedings of the International Joint Conference on Autonomous Agents and Multiagent Systems (AAMAS).
