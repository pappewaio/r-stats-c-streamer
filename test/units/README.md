# Unit Test Framework

This directory contains individual unit tests for r-stats-c-streamer functionality.

## Test Structure

Each test is a standalone bash script that:
- Sets up its own test environment
- Runs specific functionality
- Validates expected outputs
- Cleans up temporary files
- Returns appropriate exit codes (0 = pass, 1 = fail)

## Available Tests

| Test Script | Description |
|-------------|-------------|
| `test_version_check.sh` | Verifies version command works correctly |
| `test_linear_basic.sh` | Tests basic linear regression functions |
| `test_logistic_basic.sh` | Tests basic logistic regression functions |
| `test_neglog10p_handling.sh` | Tests -log10 p-value flag functionality |
| `test_column_replacement.sh` | Tests column replacement feature |
| `test_case_control_af.sh` | Tests case/control allele frequency calculation |
| `test_precision_fix.sh` | Tests high-precision p-value conversion |

## Running Tests

### Run All Tests
```bash
# From project root
./scripts/test.sh
```

### Run Individual Test
```bash
# From project root
./test/units/test_linear_basic.sh
```

## Test Output Format

Each test provides:
- ✅ **PASS** or ❌ **FAIL** status
- Detailed validation messages
- Specific feature confirmations (e.g., scientific notation detection)

## Adding New Tests

To add a new unit test:

1. Create a new script: `test/units/test_your_feature.sh`
2. Follow the existing template structure:
   ```bash
   #!/usr/bin/env bash
   # Unit test: Your Feature Description
   
   set -euo pipefail
   
   TEST_NAME="Your Feature Description"
   SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
   PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
   cd "$PROJECT_ROOT"
   
   echo "Running test: $TEST_NAME"
   
   # Your test logic here
   
   if [test_condition]; then
       echo "✅ PASS: $TEST_NAME"
       exit 0
   else
       echo "❌ FAIL: $TEST_NAME"
       exit 1
   fi
   ```
3. Make it executable: `chmod +x test/units/test_your_feature.sh`
4. The main test runner will automatically discover and run it

## Test Guidelines

- **Isolation**: Each test should be independent and not rely on other tests
- **Cleanup**: Always clean up temporary files created during testing
- **Validation**: Test both success conditions and expected output format
- **Error Handling**: Use `set -euo pipefail` for robust error handling
- **Descriptive Output**: Provide clear pass/fail messages with specific details 