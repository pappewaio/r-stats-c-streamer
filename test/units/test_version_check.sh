#!/usr/bin/env bash
# Unit test: Version Check

set -euo pipefail

TEST_NAME="Version Check"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
cd "$PROJECT_ROOT"

echo "Running test: $TEST_NAME"

# Check version output
VERSION_RESULT=$(./build/r-stats-c-streamer --version 2>&1)
VERSION_EXIT_CODE=$?

if [ $VERSION_EXIT_CODE -eq 0 ]; then
    if echo "$VERSION_RESULT" | grep -q "r-stats-c-streamer 1\.4\.1"; then
        echo "✅ PASS: $TEST_NAME"
        echo "   - Version command works correctly"
        echo "   - Version: $VERSION_RESULT"
        exit 0
    else
        echo "❌ FAIL: $TEST_NAME"
        echo "Expected version 1.4.1, got: $VERSION_RESULT"
        exit 1
    fi
else
    echo "❌ FAIL: $TEST_NAME"
    echo "Version command failed with exit code: $VERSION_EXIT_CODE"
    echo "Output: $VERSION_RESULT"
    exit 1
fi 