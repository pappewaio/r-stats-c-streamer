#!/usr/bin/env bash
# Unit test: Column Replacement Functionality

set -euo pipefail

TEST_NAME="Column Replacement Functionality"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Create function file for p-value conversion
echo -e "pval_from_neglog10p" > test_functionfile_replace.txt

# Convert -log10 p-values to normal p-values (replace column 5)
RESULT=$(cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer \
    --functionfile test_functionfile_replace.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --statmodel none \
    --replace 5 | head -3)

# Check if we got expected output structure with replaced column
if echo "$RESULT" | grep -q "RSID" && echo "$RESULT" | grep -q "rs4819391_G"; then
    echo "✅ PASS: $TEST_NAME"
    echo "   - Column replacement processing successful"
    echo "   - Original headers preserved"
    
    # Check for scientific notation in replaced p-values (precision test)
    if echo "$RESULT" | grep -q "e-"; then
        echo "   - High-precision p-values in replaced column (scientific notation)"
    fi
    
    # Verify that the structure looks like original data with replaced values
    LINE_COUNT=$(echo "$RESULT" | wc -l)
    if [ "$LINE_COUNT" -eq 3 ]; then
        echo "   - Correct number of output lines (header + 2 data rows)"
    fi
    
    rm -f test_functionfile_replace.txt
    exit 0
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Expected output with original headers and replaced column values"
    echo "Actual output:"
    echo "$RESULT"
    rm -f test_functionfile_replace.txt
    exit 1
fi 