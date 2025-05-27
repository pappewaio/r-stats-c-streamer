#!/usr/bin/env bash
# r-stats-c-streamer Test Suite Runner
# Runs all unit tests and reports results

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Make sure we are in the project root directory
SCRIPT_DIR=$(dirname "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )")
cd "$SCRIPT_DIR"

echo -e "${BLUE}r-stats-c-streamer Test Suite${NC}"
echo "=================================="

# Build the project first
echo -e "${YELLOW}Building project...${NC}"
if ! ./build.sh > /dev/null 2>&1; then
    echo -e "${RED}❌ Build failed!${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Build successful${NC}"
echo

# Initialize counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
FAILED_TEST_NAMES=()

# Function to run a single test
run_test() {
    local test_script="$1"
    local test_name=$(basename "$test_script" .sh)
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Running: $test_name${NC}"
    
    if "$test_script"; then
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo
    else
        FAILED_TESTS=$((FAILED_TESTS + 1))
        FAILED_TEST_NAMES+=("$test_name")
        echo -e "${RED}Test failed: $test_name${NC}"
        echo
    fi
}

# Run all unit tests
echo -e "${YELLOW}Running unit tests...${NC}"
echo

# Check if unit tests directory exists
if [ ! -d "test/units" ]; then
    echo -e "${RED}❌ Unit tests directory not found: test/units${NC}"
    exit 1
fi

# Run each unit test
for test_script in test/units/test_*.sh; do
    if [ -f "$test_script" ]; then
        run_test "$test_script"
    fi
done

# Summary
echo "=================================="
echo -e "${BLUE}Test Suite Summary${NC}"
echo "=================================="
echo "Total tests: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo
    echo -e "${RED}❌ Failed tests:${NC}"
    for failed_test in "${FAILED_TEST_NAMES[@]}"; do
        echo -e "  ${RED}- $failed_test${NC}"
    done
    echo
    echo -e "${RED}Some tests failed. Please check the output above.${NC}"
    exit 1
fi
