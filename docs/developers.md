# Developer specifics

## Version
Version number should be updated in:
- src/source.c
- README.md 
- CHANGELOG.md

## Automatic testing
To be done

## Manual testing
Code that cannot be made automatic, or code that hasn't gotten automatic tests yet.

### Test that NA support works
Right now there is support only for "NA" to represent missing information.
```
# Make test data
cat <<EOF > test_case_NA.txt
RSID	B	SE	Z	P	N	AFREQ
rs4819391_G	0.233253438056324	0.127172429896691	1.83415098890387	0.0668444836995174	1397	NA
rs11089128_G	-0.057230689617411	0.0707237246183849	-0.809214869921227	0.418529557835942	1397	NA
rs7288972_C	-0.0603896529952508	0.0561292812495112	-1.07590283807129	0.282156749586622	NA	0.166428060128848
rs2032141_A	2.04506156636745	0.987859595989405	2.07019456476422	NA	1397	0.000357909806728704
rs11912265_C	0.0730128523576906	0.148272187537675	NA	0.622496796832969	1397	0.0164638511095204
rs4321465_T	0.0184879350623236	NA	0.0558985901144536	0.955430598683162	1397	0.00322118826055834
rs11167319_G	NA	0.0539111323459842	-0.946776039745882	0.343916880859673	1397	0.134216177523264
rs8138488_C	-0.0636612849960759	0.0377416410861792	-1.68676515286423	0.0918721012352699	1397	0.374015748031496
rs2186521_A	0.0549469615715985	0.060835465298906	0.903206070696177	0.366572470028154	1397	0.10773085182534
EOF

# Make test data, functions to apply
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

# Run code
cat test_case_NA.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin | head | column -t

```

### Test that it produces same result as companion R script performing the same operations
The primary goal here is to test if there are any values different in the C and R versions of the script, and if there are describe then using different tolerance thresholds. This is ok for most variables, but for p-values it is not enough to know that also the very low p-values are ok. Another interesting test is to correlate the different version of inference for the same variable, i.e., are all three different variants of computing the zscore giving the same result? And does it correlate against the true variable.
```
# Run equivalent R code  - linear regression
# needs packages: getopt
cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7  --statmodel lin | head

# Run equivalent R code - logistic regression (because of `| head` we get Error: ignoring SIGPIPE signal)
cat test/testdata/logistic_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile_logistic.txt --skiplines 1 --index 1 --pvalue 9 --beta 3 --standarderror 4 --Nindividuals 10 --Ncases 11 --Ncontrols 12 --zscore 8 --allelefreq 13 --oddsratio 5 --ORu95 6 --ORl95 7 --statmodel log | head

# Test diff of values using tolerance thresholds (linear)
mkdir -p test/out
cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7  --statmodel lin > test/out/r_version
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin > test/out/c_version
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

# Test diff of values using tolerance thresholds (logistic)
mkdir -p test/out
cat test/testdata/logistic_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile_logistic.txt --skiplines 1 --index 1 --pvalue 9 --beta 3 --standarderror 4 --Nindividuals 10 --Ncases 11 --Ncontrols 12 --zscore 8 --allelefreq 13 --oddsratio 5 --ORu95 6 --ORl95 7 --statmodel log > test/out/r_version_2
cat test/testdata/logistic_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile_logistic.txt --skiplines 1 --index 1 --pvalue 9 --beta 3 --standarderror 4 --Nindividuals 10 --Ncases 11 --Ncontrols 12 --zscore 8 --allelefreq 13 --oddsratio 5 --ORu95 6 --ORl95 7 --statmodel log > test/out/c_version_2
./test/compare_r_and_c.sh test/out/c_version_2 test/out/r_version_2
###OK: Same number of columns in both files
###values with diff tolerance: 0.000001
###41
###values with diff tolerance: 0.00001
###38
###values with diff tolerance: 0.0001
###32
###values with diff tolerance: 0.001
###0

#test neglog10p flag
cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 --statmodel lin --neglog10p | head | column -t
cat test/testdata/linear_testStats_neglog10Pvalue.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7  --statmodel lin --neglog10p | head

```

## valgrind testing
use the valgrind software to search for memory leaks

### Test that it doesnt leak memory using valgrind

To identify reserved memory not freed and general bad memory handling we can use valgrind (which you have to install)

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

## Performance
The main purpose of this software is to gain speed while keeping memory usage low. Here is a direct comparison to R.


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

