# r-stats-c-streamer

An efficient way of doing typical statistical conversions and mathematical operations, using the internal C implementations from R without using R code to access them.

**Version:** 1.4.2

## Overview

r-stats-c-streamer is a command-line tool that performs statistical transformations on tabular data using R's internal C functions. It's designed for high-performance processing of GWAS summary statistics and other genomic data.

### Key Features

- **High Performance**: Uses R's internal C implementations without R overhead
- **Stream Processing**: Unix-style pipeline processing for large datasets
- **Statistical Models**: Support for linear and logistic regression statistics
- **Precision**: High-precision p-value calculations (scientific notation)
- **Flexible**: Column replacement and transformation capabilities
- **Container Ready**: Singularity container support for HPC environments

## Quick Start

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt install cmake

# Install rmath library
git clone https://github.com/statslabs/rmath.git
cd rmath && mkdir build && cd build
cmake .. && make && sudo make install
```

### Installation

```bash
# Clone and build
git clone <repository-url>
cd r-stats-c-streamer
./build.sh

# Verify installation
./build/r-stats-c-streamer --version
```

### Basic Usage

```bash
# Create a function file
echo -e "zscore_from_beta_se\npval_from_zscore" > functions.txt

# Process your data
cat your_data.txt | ./build/r-stats-c-streamer \
    --functionfile functions.txt \
    --skiplines 1 \
    --index 1 \
    --beta 2 \
    --standarderror 3 \
    --statmodel lin
```

## Core Functionality

### Statistical Models
- **Linear regression** (`--statmodel lin`): GWAS summary statistics
- **Logistic regression** (`--statmodel log`): Case-control studies  
- **None** (`--statmodel none`): Utility functions only

### Common Functions
- `zscore_from_beta_se`: Calculate z-score from beta and standard error
- `pval_from_zscore`: Calculate p-value from z-score
- `pval_from_neglog10p`: Convert -log10 p-values to normal p-values
- `AF_from_CaseAF_ControlAF`: Calculate overall allele frequency from case/control AFs

### Special Features

#### High-Precision P-values
Version 1.4.0+ includes fixes for highly significant p-values that were previously truncated:
```bash
# Handles very small p-values correctly (e.g., 1e-12 instead of 0.000000)
cat data.txt | ./build/r-stats-c-streamer --functionfile functions.txt --neglog10p
```

#### Column Replacement
Replace specific columns with calculated values:
```bash
# Replace column 5 with converted p-values
cat data.txt | ./build/r-stats-c-streamer \
    --functionfile functions.txt \
    --replace 5 \
    --statmodel none
```

## Documentation

- **[Complete Testing Guide](docs/testing_examples.md)**: Comprehensive examples and usage patterns
- **[Developer Documentation](docs/developers.md)**: Development and testing specifics
- **[Precision Fix Details](docs/test_neglog10p_fix.md)**: Information about the p-value precision bug fix
- **[Changelog](CHANGELOG.md)**: Version history and changes

## Quick Test

```bash
# Run a simple test to verify everything works
echo -e "zscore_from_beta_se" > test_functions.txt
echo -e "test_variant\t0.1\t0.05" | ./build/r-stats-c-streamer \
    --functionfile test_functions.txt \
    --skiplines 0 \
    --index 1 \
    --beta 2 \
    --standarderror 3 \
    --statmodel lin
# Expected output: test_variant    2.000000e+00
```

## Development Workflow

### Before Committing to Main Branch

**Always run the complete test suite before committing to ensure code quality:**

```bash
# Run all tests (builds project automatically)
./scripts/test.sh
```

This will:
- Build the project from scratch
- Execute all 7 unit tests
- Report detailed results with pass/fail status
- Ensure all functionality works correctly

**Only commit if all tests pass (🎉 All tests passed!)**

### Individual Testing

For development and debugging, you can run specific tests:

```bash
# Test specific functionality
./test/units/test_linear_basic.sh
./test/units/test_precision_fix.sh

# Test after making changes to column replacement
./test/units/test_column_replacement.sh
```

## Support

- **Issues**: Report bugs and feature requests via GitHub issues
- **Testing**: Run `./scripts/test.sh` to execute the complete test suite
- **Version**: Check your version with `./build/r-stats-c-streamer --version`

## License

[Add your license information here]

---

For detailed examples, advanced usage, and comprehensive testing instructions, see the [Testing Examples Guide](docs/testing_examples.md).
