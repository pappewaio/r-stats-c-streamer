file1=$1
file2=$2

#how many cols in each file?
fc1=$(head -n1 ${file1} | awk '{print NF}')
fc2=$(head -n1 ${file1} | awk '{print NF}')

#Do we have same nr of columns?
if [ "${fc1}" == "${fc2}" ]; then
  echo "OK: Same number of columns in both files "
else
  echo "FAIL: Not same number of columns in both files, ${fc1} != ${fc2} "
fi

#merge using paste
tolerance=0.000001
echo "values with diff tolerance: $tolerance"
paste $file1 $file2 | awk -vlim="${fc1}" -vtol="${tolerance}" '{for (i=1; i<=lim; i++){if($i-$(i+lim)>tol){print NR, $i,$(i+lim)}}}' | wc -l
tolerance=0.00001
echo "values with diff tolerance: $tolerance"
paste $file1 $file2 | awk -vlim="${fc1}" -vtol="${tolerance}" '{for (i=1; i<=lim; i++){if($i-$(i+lim)>tol){print NR, $i,$(i+lim)}}}' | wc -l
tolerance=0.0001
echo "values with diff tolerance: $tolerance"
paste $file1 $file2 | awk -vlim="${fc1}" -vtol="${tolerance}" '{for (i=1; i<=lim; i++){if($i-$(i+lim)>tol){print NR, $i,$(i+lim)}}}' | wc -l
tolerance=0.001
echo "values with diff tolerance: $tolerance"
paste $file1 $file2 | awk -vlim="${fc1}" -vtol="${tolerance}" '{for (i=1; i<=lim; i++){if($i-$(i+lim)>tol){print NR, $i,$(i+lim)}}}' | wc -l


