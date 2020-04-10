### Based on this repo
https://github.com/statslabs/rmath

### Build project 
Do this in same folder as CMakeLists.txt & source.c

```
mkdir build && cd build
cmake ..
make

#test
echo -e "0.9\n0.5\n0.1e-10" | ./qnorm_stdin

```

### Build singularity image
First make sure singularity is installed

```
fname="$(date +%F)"-ubuntu-1804_qnorm-stdin.simg
sudo singularity build ${fname} ubuntu-18.04_qnorm-stdin.def 


# check that image is executable and then test it 
echo -e "0.9\n0.5\n0.1e-10" | ./2020-04-09-ubuntu-1804_qnorm-stdin.simg qnorm_stdin
```

