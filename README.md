### The utilization of cmake and the rmath library (prerequisites)
- (on Ubuntu): sudo apt install cmake
- https://github.com/statslabs/rmath

### Build project and test on test data
Do this in same folder as CMakeLists.txt & source.c

```
# Build project (it will remove old build folders)
./build.sh

# look at test data using head.
cat test/testdata/linear_testStats.txt | head | column -t

# create file with functions to apply on each row
echo -e "zscore_from_pval_beta
zscore_from_pval_beta_N
zscore_from_beta_se
pval_from_zscore_N
pval_from_zscore
beta_from_zscore_se
beta_from_zscore_N_af
se_from_zscore_beta
se_from_zscore_N_af
N_from_zscore_beta_af"> functiontestfile.txt

# Test program
cat test/testdata/linear_testStats.txt | ./build/stat_r_in_c --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 | head
##0	pval_beta_2_zscore	zscore_N_2_pvalue	zscore_se_2_beta
##1	1.281552	0.317317	-1.200000
##2	0.000000	0.021453	2.990000
##3	6.706023	0.548509	0.660000

```

### Test that it doesnt leak memory using valgrind

### Test that it produces same result as companion R script performing the same operations
```
cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 | head

```

### Build singularity image
First make sure singularity is installed

```
# Make singularity image based on defintion file
fname="$(date +%F)"-ubuntu-1804_stat_r_in_c.simg
sudo singularity build ${fname} ubuntu-18.04_stat_r_in_c.def 


# Check that image is executable and then test it (change date)
cat rinc_testdata | ./20xx-xx-xx-ubuntu-1804_stat_r_in_c.simg stat_r_in_c --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 2 --oddsratio 3 --allelefreq 4 --beta 5 --standarderror 6 --Nindividuals 7 --zscore 8

```

### Generate 1 million rows test data using R 
```
./gentestdata.sh 1000000 > testdata


```
