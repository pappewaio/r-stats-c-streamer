# r-stats-c-streamer
An efficient way of doing typical stat conversions or other math operations, using the internal C implementations from R without using R code to access them.

version:1.2.2

### The utilization of cmake and the rmath library (prerequisites)
- (on Ubuntu): sudo apt install cmake
- https://github.com/statslabs/rmath

### Build project and test on test data
Do this in same folder as CMakeLists.txt & source.c

```
# Build project (it will remove old build folders)
./build.sh

# Check the current version
./build/r-stats-c-streamer --version

# Look at test data using head.
cat test/testdata/linear_testStats.txt | head | column -t

# Re-create the included testfile, which specifies functions to apply on each row (linear regression)
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

# Re-create the included testfile, which specifies functions to apply on each row (linear regression)
echo -e "zscore_from_beta_se
zscore_from_pval_oddsratio
pval_from_zscore
beta_from_oddsratio
beta_from_zscore_se
se_from_beta_zscore
se_from_ORu95_ORl95
Neff_from_Nca_Nco" > functiontestfile_logistic.txt

# Try program - linear model
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin | head | column -t
###0             zscore_from_pval_beta  zscore_from_pval_beta_N  zscore_from_beta_se	etc..
###rs4819391_G   1.832718               1.834151                 1.834151	etc..
###rs11089128_G  -0.808975              -0.809215                -0.809215	etc..
###rs7288972_C   -1.075487              -1.075903                -1.075903	etc..
###rs2032141_A   2.068237               2.070195                 2.070195	etc..
###etc...

# Try program - logistic model
cat test/testdata/logistic_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile_logistic.txt --skiplines 1 --index 1 --pvalue 9 --beta 3 --standarderror 4 --Nindividuals 10 --Ncases 11 --Ncontrols 12 --zscore 8 --allelefreq 13 --oddsratio 5 --ORu95 6 --ORl95 7 --statmodel log


# If the P-value uses -log10, apply the neglog10 flag
cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin --neglog10p | head | column -t
###0             zscore_from_pval_beta  zscore_from_pval_beta_N  zscore_from_beta_se	etc..
###rs4819391_G   1.832718               1.834151                 1.834151             
###rs11089128_G  -0.808975              -0.809215                -0.809215            
###rs7288972_C   -1.075487              -1.075903                -1.075903            
###rs2032141_A   2.068237               2.070195                 2.070195             
###etc...

```

As you can see the column names describes the operation performed, and using the `--neglog10` flag we get the same result as if we would have a "normal" P-value. There are some decimals that are off in this example, but that is because the test set with -log(Pvalue) is slightly truncated. In a future version the test set will contain a not truncated value.

Another important feature with this software is that it can parse a file and return the exact same columns, and replace certain columns using one of the available functions. This could be a way of managing -log10 pvalues without using the '--neglog10' flag, as we could pre-process the data and replace all -log10 pvalues with normal pvalues. See below for an example of how to do that:

```
# Re-create the included testfile, which specifies functions to apply on each row
echo -e "pval_from_neglog10p" > functiontestfile2.txt

# If the P-value uses -log10, pre-process and convert to normal pvalue
cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer --functionfile functiontestfile2.txt --skiplines 1 --index 1 --pvalue 5 --statmodel none --replace 5 | head | column -t
###RSID          B                    SE                  Z                   neglog10P  etc..
###rs4819391_G   0.233253438056324    0.127172429896691   1.83415098890387    1.17493
###rs11089128_G  -0.057230689617411   0.0707237246183849  -0.809214869921227  0.378274
###rs7288972_C   -0.0603896529952508  0.0561292812495112  -1.07590283807129   0.54951
###etc...   2.04506156636745     0.987859595989405   2.07019456476422    1.41321

```

Right now the replace functionality can only replace one column at a time, but we have left room for a future upgrade where we could replace multiple columns each using a specific function defined in the functionfile.

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

### Extra
- See [developer specifics](docs/developers.md) for testing and other non user specific stuff


