#!/usr/bin/env bash
# Unit test: High-Precision P-value Conversion

set -euo pipefail

TEST_NAME="High-Precision P-value Conversion"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Run the existing precision test
cd test/neglog10p_precision_fix
PRECISION_RESULT=$(./run_test.sh 2>&1)
PRECISION_EXIT_CODE=$?
cd "$PROJECT_ROOT"

if [ $PRECISION_EXIT_CODE -eq 0 ]; then
    echo "✅ PASS: $TEST_NAME"
    echo "   - Precision fix test suite passed"
    
    # Check for specific precision indicators
    if echo "$PRECISION_RESULT" | grep -q "e-"; then
        echo "   - High-precision scientific notation detected"
    fi
    
    if echo "$PRECISION_RESULT" | grep -q "rs_high_sig"; then
        echo "   - High significance variants processed correctly"
    fi
    
    exit 0
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Precision fix test suite failed"
    echo "Output:"
    echo "$PRECISION_RESULT"
    exit 1
fi 