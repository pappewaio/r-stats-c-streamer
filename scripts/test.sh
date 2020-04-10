#!/usr/bin/env bash
set -euo pipefail

# Make sure we are in the directory of the script.
script_dir=$(dirname "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )")
cd $script_dir

mkdir -p build
cd build
cmake ..
make

#echo -e "0.9\n0.5\n0.1e-10" | \
#    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./qnorm_stdin qnorm
echo -e "0.9\n0.5\n0.1e-10" | ./qnorm_stdin qnorm
