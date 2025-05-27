#!/usr/bin/env bash
# Unit test: Basic Logistic Regression Statistics

set -euo pipefail

TEST_NAME="Logistic Regression Basic Functions"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Create function file for logistic regression
echo -e "zscore_from_beta_se
zscore_from_pval_oddsratio
pval_from_zscore
beta_from_oddsratio
beta_from_zscore_se
se_from_beta_zscore
se_from_ORu95_ORl95
Neff_from_Nca_Nco" > test_functionfile_logistic.txt

# Test logistic model
RESULT=$(cat test/testdata/logistic_testStats.txt | ./build/r-stats-c-streamer \
    --functionfile test_functionfile_logistic.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 9 \
    --beta 3 \
    --standarderror 4 \
    --Nindividuals 10 \
    --Ncases 11 \
    --Ncontrols 12 \
    --zscore 8 \
    --allelefreq 13 \
    --oddsratio 5 \
    --ORu95 6 \
    --ORl95 7 \
    --statmodel log | head -2)

# Check if we got expected output structure
if echo "$RESULT" | grep -q "zscore_from_beta_se" && echo "$RESULT" | grep -q "rs4819391"; then
    echo "✅ PASS: $TEST_NAME"
    echo "   - Function headers generated correctly"
    echo "   - Logistic regression processing successful"
    
    # Check for scientific notation in p-values (precision test)
    if echo "$RESULT" | grep -q "e-"; then
        echo "   - High-precision p-values detected (scientific notation)"
    fi
    
    rm -f test_functionfile_logistic.txt
    exit 0
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Expected output with function headers and variant data"
    echo "Actual output:"
    echo "$RESULT"
    rm -f test_functionfile_logistic.txt
    exit 1
fi 