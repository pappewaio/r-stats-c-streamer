### The utilization of cmake and the rmath library
- (on Ubuntu): sudo apt install cmake
- https://github.com/statslabs/rmath

### Build project 
Do this in same folder as CMakeLists.txt & source.c

```
# Build project (it will remove old build folders)
./build.sh

# create test data using echo command.
echo -e "0\tpval\tOR\tAF\tbeta\tSE\tN\tzscore
1\t0.9\t1.2\t0.4\t1.3\t0.2\t40000\t-1.0
2\t0.5\t1.3\t0.6\t3.3\t0.15\t45000\t2.3
3\t0.1e-10\t1.1\t0.1\t-0.9\t0.1\t50000\t0.6" > rinc_testdata

# create file with functions to apply on each row
echo -e "pval_oddsratio_2_zscore
pval_beta_2_zscore
zscore_N_2_pvalue
zscore_se_2_beta" > functiontestfile.txt

# Test program
cat rinc_testdata | ./build/stat_r_in_c --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 2 --oddsratio 3 --allelefreq 4 --beta 5 --standarderror 6 --Nindividuals 7 --zscore 8

```

### Build singularity image
First make sure singularity is installed

```
# Make singularity image based on defintion file
fname="$(date +%F)"-ubuntu-1804_stat_r_in_c.simg
sudo singularity build ${fname} ubuntu-18.04_stat_r_in_c.def 


# Check that image is executable and then test it (change date)
echo -e "0.9\n0.5\n0.1e-10" | ./20xx-xx-xx-ubuntu-1804_stat_r_in_c.simg stat_r_in_c qnorm
```

### Generate 1 million rows test data
```
./gentestdata.sh 1000000 > testdata

