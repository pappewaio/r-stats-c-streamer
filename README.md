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

# Re-create the included testfile, which specifies functions to apply on each row
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

# Try program
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin | head | column -t
###0             zscore_from_pval_beta  zscore_from_pval_beta_N  zscore_from_beta_se	etc..
###rs4819391_G   1.832718               1.834151                 1.834151	etc..
###rs11089128_G  -0.808975              -0.809215                -0.809215	etc..
###rs7288972_C   -1.075487              -1.075903                -1.075903	etc..
###rs2032141_A   2.068237               2.070195                 2.070195	etc..
###etc...

```

### Test that it doesnt leak memory using valgrind

To identify reserved memroy not freed and general bad memory handling we can use valgrind

```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose \
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin | head | column -t

#If all went well, then these would be the last lines
==35047== HEAP SUMMARY:
==35047==     in use at exit: 0 bytes in 0 blocks
==35047==   total heap usage: 46 allocs, 46 frees, 1,060,870 bytes allocated
==35047== 
==35047== All heap blocks were freed -- no leaks are possible
==35047== 
==35047== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

```

### Test that it produces same result as companion R script performing the same operations
The primary goal here is to test if there are any values different in the C and R versions of the script, and if there are describe then using different tolerance thresholds. This is ok for most variables, but for p-values it is not enough to know that also the very low p-values are ok. Another interesting test is to correlate the different version of inference for the same variable, i.e., are all three different variants of computing the zscore giving the same result? And does it correlate against the true variable. 
```
# Run equivalent R code
cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7  --statmodel lin| head

# Test diff of values using tolerance thresholds
mkdir -p test/out
cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7  --statmodel lin> test/out/r_version
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 > test/out/c_version
./test/compare_r_and_c.sh test/out/c_version test/out/r_version
###OK: Same number of columns in both files 
###values with diff tolerance: 0.000001
###41
###values with diff tolerance: 0.00001
###38
###values with diff tolerance: 0.0001
###32
###values with diff tolerance: 0.001
###0


```


### Generate 1 000 000 rows test and timestamp the C and R versions
```
# Initiate large gwas file
cat test/testdata/linear_testStats.txt >  test/out/testdata_100000_rows.txt

# Amplify file to 1 000 000 rows
for i in {2..1000};do
  tail -n+2 test/testdata/linear_testStats.txt
done >> test/out/testdata_100000_rows.txt

time cat test/out/testdata_100000_rows.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin > test/out/r_version2
###real	1m5,043s
###user	0m6,490s
###sys	0m55,129s


time cat test/out/testdata_100000_rows.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin > test/out/c_version2
###real	0m0,753s
###user	0m0,610s
###sys	0m0,087s

```

### Build singularity image
First make sure singularity is installed. Then if you are satisfied with the tests above you can proceed and build the image, which will make it possible to directly run on for example a high-performance cluster without admin privileges. 

```
# Make singularity image based on defintion file
mkdir -p images
fname="$(date +%F)"-ubuntu-2004_r-stats-c-streamer.simg
sudo singularity build images/${fname} ubuntu-20.04_r-stats-c-streamer.def 

# Check that image is executable and then test it (change date)
cat test/testdata/linear_testStats.txt | singularity run --bind .:/mnt images/2020-12-14-ubuntu-2004_r-stats-c-streamer.simg r-stats-c-streamer --functionfile  /mnt/functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin | head | column -t

```

