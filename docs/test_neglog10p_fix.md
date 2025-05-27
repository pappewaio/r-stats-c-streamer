# Bug Fix: Conversion of p-values from -log10 scale produces incorrect p-values

## Problem Description

The `pval_from_neglog10p` function was producing incorrect p-values for highly significant variants. Specifically:

- Highly significant p-values (e.g., -log10 p-values > ~6) were being truncated to 0.000000
- This was causing substantial inaccuracies in downstream analyses
- The most significant p-values became 0, while less significant ones became 1E-6

## Root Cause

The issue was in the `none_pval_from_neglog10p` function in `src/stat_operators.c`. The function was using:

```c
printf("%lf", pow(10, -prob));
```

The `%lf` format specifier has limited precision (typically 6 decimal places by default), so very small numbers like `10^(-11.2982) ≈ 5.03 × 10^(-12)` were being rounded to 0.000000.

## Solution

Changed the printf format specifier to use scientific notation with higher precision:

```c
printf("%.15e", pow(10, -prob));
```

This change was also applied to other p-value output functions:
- `lin_operator_pval_from_zscore_N`
- `lin_operator_pval_from_zscore`  
- `log_operator_pval_from_zscore`

## Test Results

### Before Fix:
```
test_variant    11.2982 → 0.000000
test_variant    7.14186 → 0.000000
test_variant    7.20949 → 0.000000
```

### After Fix:
```
test_variant    11.2982 → 5.032687915706674e-12
test_variant    7.14186 → 7.213399742411434e-08
test_variant    7.20949 → 6.173195060319592e-08
```

## Verification

The fix was verified by:
1. Testing with the original problematic data from `test/testdata/linear_testStats_neglog10Pvalue.txt`
2. Comparing results with R's conversion: `10^(-11.2982)` = `5.032687915706674e-12`
3. Testing that normal p-values still work correctly
4. Testing that p-value calculations from z-scores also use high precision

## Files Modified

- `src/stat_operators.c`: Updated printf format specifiers in p-value output functions 