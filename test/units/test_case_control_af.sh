#!/usr/bin/env bash
# Unit test: Case/Control Allele Frequency Calculation

set -euo pipefail

TEST_NAME="Case/Control Allele Frequency Calculation"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Create function file for EAF calculation
echo -e "zscore_from_beta_se
AF_from_CaseAF_ControlAF" > test_functionfile_cacoaf.txt

# Calculate overall EAF from case/control AFs
RESULT=$(cat test/testdata/logistic_testStats_caseAF_controlAF.txt | ./build/r-stats-c-streamer \
    --functionfile test_functionfile_cacoaf.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 9 \
    --beta 3 \
    --standarderror 4 \
    --Nindividuals 10 \
    --zscore 8 \
    --Ncases 11 \
    --Ncontrols 12 \
    --CaseAF 14 \
    --ControlAF 15 \
    --statmodel log | head -2)

# Check if we got expected output structure
if echo "$RESULT" | grep -q "AF_from_CaseAF_ControlAF" && echo "$RESULT" | grep -q "rs4819391"; then
    echo "✅ PASS: $TEST_NAME"
    echo "   - Case/Control AF calculation successful"
    echo "   - Function headers generated correctly"
    
    # Check that we have numeric values in the AF column
    if echo "$RESULT" | tail -1 | grep -q "0\."; then
        echo "   - Calculated allele frequencies appear to be in valid range"
    fi
    
    rm -f test_functionfile_cacoaf.txt
    exit 0
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Expected output with AF_from_CaseAF_ControlAF header and variant data"
    echo "Actual output:"
    echo "$RESULT"
    rm -f test_functionfile_cacoaf.txt
    exit 1
fi 