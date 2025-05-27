# Testing neglog10p Precision Fix

## Bug Description
The `pval_from_neglog10p` function was producing incorrect results for highly significant p-values. When converting from -log10 scale, very small p-values (e.g., those with -log10 p > 6) were being truncated to 0.000000 instead of showing the correct small values.

## Root Cause
The issue was in the printf format specifier used in the `none_pval_from_neglog10p` function:

```c
// Before fix (caused truncation):
printf("%lf", pow(10, -prob));

// After fix (scientific notation with 5 digits precision):
printf("%.5e", pow(10, -prob));
```

## Functions Fixed
The precision fix was applied to all p-value output functions:
- `none_pval_from_neglog10p` 
- `lin_operator_pval_from_zscore_N`
- `lin_operator_pval_from_zscore`
- `log_operator_pval_from_zscore`

## Test Results
With the fix, highly significant p-values are now correctly converted:

| Input (-log10 p) | Before Fix | After Fix |
|------------------|------------|-----------|
| 11.2982 | 0.000000 | 5.03269e-12 |
| 7.14186 | 0.000000 | 7.21687e-08 |
| 7.20949 | 0.000000 | 6.17284e-08 |

## Testing
The fix includes a comprehensive test suite in `test/neglog10p_precision_fix/` that verifies:
- Correct conversion of high -log10 p-values
- Preservation of existing functionality
- Scientific notation output format with appropriate precision

## Implementation Details
The fix uses scientific notation (%.5e) which provides 5 digits of precision, sufficient for most statistical applications while avoiding the overkill of 15 digits that was initially implemented.

## Verification

The fix was verified by:
1. Testing with the original problematic data from `test/testdata/linear_testStats_neglog10Pvalue.txt`
2. Comparing results with R's conversion: `10^(-11.2982)` = `5.032687915706674e-12`
3. Testing that normal p-values still work correctly
4. Testing that p-value calculations from z-scores also use high precision

## Files Modified

- `src/stat_operators.c`: Updated printf format specifiers in p-value output functions 