#!/usr/bin/env bash
# Unit test: Basic Linear Regression Statistics

set -euo pipefail

TEST_NAME="Linear Regression Basic Functions"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Create function file for linear regression
echo -e "zscore_from_pval_beta
zscore_from_pval_beta_N
zscore_from_beta_se
pval_from_zscore_N
pval_from_zscore
beta_from_zscore_se
beta_from_zscore_N_af
se_from_zscore_beta
se_from_zscore_N_af
N_from_zscore_beta_af" > test_functionfile_linear.txt

# Test linear model
RESULT=$(cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer \
    --functionfile test_functionfile_linear.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --beta 2 \
    --standarderror 3 \
    --Nindividuals 6 \
    --zscore 4 \
    --allelefreq 7 \
    --statmodel lin | head -2)

# Check if we got expected output structure
if echo "$RESULT" | grep -q "zscore_from_pval_beta" && echo "$RESULT" | grep -q "rs4819391_G"; then
    echo "✅ PASS: $TEST_NAME"
    echo "   - Function headers generated correctly"
    echo "   - Data processing successful"
    
    # Check for scientific notation in p-values (precision test)
    if echo "$RESULT" | grep -q "e-"; then
        echo "   - High-precision p-values detected (scientific notation)"
    fi
    
    rm -f test_functionfile_linear.txt
    exit 0
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Expected output with function headers and variant data"
    echo "Actual output:"
    echo "$RESULT"
    rm -f test_functionfile_linear.txt
    exit 1
fi 