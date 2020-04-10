#!/usr/bin/env bash
set -euo pipefail

# Make sure we are in the directory of the script.
script_dir=$(dirname "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )")
cd $script_dir

mkdir -p build
cd build
cmake ..
make

valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes ./qnorm_stdin qnorm < bench.dat > out.log
