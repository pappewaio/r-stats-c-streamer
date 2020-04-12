### Based on this repo
https://github.com/statslabs/rmath

### Build project 
Do this in same folder as CMakeLists.txt & source.c

```
mkdir build && cd build
cmake ..
make

#test
echo -e "0.9\n0.5\n0.1e-10" | ./stat_r_in_c qnorm

```

### Build singularity image
First make sure singularity is installed

```
fname="$(date +%F)"-ubuntu-1804_stat_r_in_c.simg
sudo singularity build ${fname} ubuntu-18.04_stat_r_in_c.def 


# check that image is executable and then test it (change date)
echo -e "0.9\n0.5\n0.1e-10" | ./20xx-xx-xx-ubuntu-1804_stat_r_in_c.simg stat_r_in_c qnorm
```

### Generate 1 million rows test data
```
./gentestdata.sh 1000000 > testdata

