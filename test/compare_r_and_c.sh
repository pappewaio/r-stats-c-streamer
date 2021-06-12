#cat test/testdata/linear_testStats.txt | Rscript test/calc_linear_functions.R --functionfile  functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 > test/out/r_version
#cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer --functionfile functiontestfile.txt --skiplines 1 --index 1 --pvalue 5 --beta 2 --standarderror 3 --Nindividuals 6 --zscore 4 --allelefreq 7 > test/out/c_version
file1=$1
file2=$2
#file1="test/out/r_version"
#file2="test/out/c_version"

# How many cols in each file?
fc1=$(head -n1 ${file1} | awk '{print NF}')
fc2=$(head -n1 ${file1} | awk '{print NF}')

# Do we have same nr of columns?
if [ "${fc1}" == "${fc2}" ]; then
echo "################################################################################"
  echo "OK: Same number of columns in r and c outputfiles "
else
  echo "FAIL: Not same number of columns in both files, ${fc1} != ${fc2} "
fi
# Check diffs using awk
head -n1 $file1 > test/out/c_r_diff
paste $file1 $file2 | awk -vlim="${fc1}" 'NR>1{for (i=1; i<=(lim); i++){diff=($i-$(i+lim)); printf "%s\t", diff}; printf "\n"}' >> test/out/c_r_diff

# Count diffs for different thresholds
echo ""
echo "################################################################################"
echo "The distribution of the deviations from zero:"
cat test/out/c_r_diff | awk '
NR>1{for (i=1; i<=NF; i++){
if($i>0.01){seen["tol_0.01"] += 1}
if($i>0.001){seen["tol_0.001"] += 1}
if($i>0.0001){seen["tol_0.0001"] += 1}
if($i>0.00001){seen["tol_0.00001"] += 1}
if($i>0.000001){seen["tol_0.000001"] += 1}
if($i>0.0000001){seen["tol_0.0000001"] += 1}
}};
END{for (i in seen) print seen[i],i}
' | sort -k2,2

echo ""
echo "################################################################################"
echo "To investigate the distribution of the deviations from zero, when comparing the R and C version"
echo "different difference tolerance summary metrics were calculated:"
cat test/out/c_r_diff | awk '
BEGIN{getline;
for (i=1; i<=NF; i++){
  header[i] = $i;
}};
NR>1{for (i=1; i<=NF; i++){
if($i>0.01){seen1[i] += 1}
if($i>0.001){seen2[i] += 1}
if($i>0.0001){seen3[i] += 1}
if($i>0.00001){seen4[i] += 1}
if($i>0.000001){seen5[i] += 1}
if($i>0.0000001){seen6[i] += 1}
}};
END{
print "\ntol_0.01";for (i in seen1) print "*", seen1[i],header[i];
print "\ntol_0.001";for (i in seen2) print "*", seen2[i],header[i];
print "\ntol_0.0001";for (i in seen3) print "*", seen3[i],header[i];
print "\ntol_0.00001";for (i in seen4) print "*", seen4[i],header[i];
print "\ntol_0.000001";for (i in seen5) print "*", seen5[i],header[i];
print "\ntol_0.0000001";for (i in seen6) print "*", seen6[i],header[i]}
' 

##merge input and inferred
paste test/testdata/linear_testStats.txt test/out/c_version >  test/out/raw_c_version

#Take out a random sample of rows to compare, less accurate for fewer rows
#Will be used to generate correlation graphs
samples_nrrows=100
perc="$(wc -l test/out/raw_c_version | awk -vsamples_nrrows=${samples_nrrows} '{print samples_nrrows/$1}')"
head -n1 test/out/raw_c_version > test/out/sample_${samples_nrrows}.txt
tail -n+2 test/out/raw_c_version | awk -vperc=${perc} 'BEGIN {srand()} !/^$/ { if (rand() <= perc) print $0}' >> test/out/sample_${samples_nrrows}.txt

echo ""
echo "################################################################################"
echo "Correlations among inferred or true variables:"
#test correlation among variables
varstocorr=(Z P B SE N)
varstocorr2=(zscore pval beta se N)

for i in {0..4}; do
  var=${varstocorr[i]}
  var2=${varstocorr2[i]}
  echo ""
  Rscript test/calc_correlations.R test/out/sample_100.txt ${var} ${var2}
  echo ""
done

