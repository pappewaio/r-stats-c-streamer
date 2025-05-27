# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.4.3] - 2025-05-27
### Changed
- **Adjusted p-value precision**: Reduced precision in p-value output from 15 to 5 digits in scientific notation (%.5e)
- Updated printf format specifiers in p-value functions based on user feedback that 15 digits was overkill
- Maintains statistical accuracy while providing more reasonable precision for typical use cases
- Affects highly significant p-values converted from -log10 scale and z-score calculations

### Functions Updated
- `none_pval_from_neglog10p`
- `lin_operator_pval_from_zscore_N` 
- `lin_operator_pval_from_zscore`
- `log_operator_pval_from_zscore`

## [1.4.2] - 2025-05-27
### Added
- **Comprehensive unit test framework** with 7 individual test scripts in `test/units/`
- **Modern test runner** (`scripts/test.sh`) with colored output and detailed reporting
- **Development workflow section** in README emphasizing mandatory testing before commits
- **Unit test documentation** in `test/units/README.md` with guidelines for adding new tests
- **Automated testing integration** in documentation with clear instructions

### Changed
- **Replaced legacy test script** with robust unit test framework
- **Enhanced testing documentation** with automated testing section
- **Improved developer guidance** with clear workflow for code quality assurance

### Tests Added
- `test_version_check.sh` - Version command validation
- `test_linear_basic.sh` - Linear regression functions
- `test_logistic_basic.sh` - Logistic regression functions
- `test_neglog10p_handling.sh` - -log10 p-value flag functionality
- `test_column_replacement.sh` - Column replacement feature
- `test_case_control_af.sh` - Case/control allele frequency calculation
- `test_precision_fix.sh` - High-precision p-value conversion

## [1.4.1] - 2025-05-27
### Changed
- **Major documentation restructuring**: Streamlined README.md to focus on essentials and quick start
- Moved comprehensive testing examples and usage patterns to `docs/testing_examples.md`
- Improved README structure with better organization of key features, installation, and basic usage
- Enhanced documentation navigation with clear links to detailed guides

### Added
- New comprehensive testing guide in `docs/testing_examples.md` with detailed examples for all features
- Complete command line options reference table
- Troubleshooting section with common issues and solutions
- Quick test example in README for immediate verification

## [1.4.0] - 2025-05-27
### Fixed
- Fixed precision issue in p-value conversion from -log10 scale where highly significant p-values (e.g., -log10 p > 6) were being truncated to 0.000000
- Updated printf format specifiers in p-value output functions to use scientific notation with higher precision (%.15e)
- Fixed `pval_from_neglog10p`, `pval_from_zscore`, `pval_from_zscore_N`, and logistic `pval_from_zscore` functions

### Added
- Comprehensive test suite for neglog10p precision fix in `test/neglog10p_precision_fix/`
- Documentation of the bug fix in `docs/test_neglog10p_fix.md`

## [1.3.0] - 2023-08-24
### Added
- New functionality to infer Effect Allele Frequency (EAF) from case/control allele frequencies
- Added `AF_from_CaseAF_ControlAF` function for logistic regression statistics
- Added `--CaseAF` and `--ControlAF` command line options (flags -y and -t)
- Added test data for case/control allele frequency functionality (`logistic_testStats_caseAF_controlAF.txt`)
- Added example usage in README for inferring EAF from case/control N and EAF

## [1.2.3] - 2022-08-10
### added
- new log10 backtransformation function

## [1.2.2] - 2022-01-24
### Fixed
- NA support 

### Changed
- Moved documentation specific to developers to docs/ 

## [1.2.1] - 2021-11-22
### Fixed
- In the README the logistic example data was given to the linear run.

## [1.2.0] - 2021-06-12
### Added
- New functionality to handle logisic regression statistics.

## [1.1.1] - 2021-05-12
### Changed
- Removed the extra newline which created and empty 2nd row.

## [1.1.0] - 2021-05-12
### Added
- New functionality to make inplace replacement. The first function available is neglog10p.

## [1.0.3] - 2021-02-11
### Changed
- Changed permission so that build.sh is executable without having to first chmod

### Added
- Swtich for -logP, which converts the P-value to a normal P-value before further calculations

## [1.0.2] - 2021-01-20
### Changed
- Changed file name for the singularity definition file from an older name of the software


## [1.0.1] - 2021-01-20
### Added
- Switch for allele frequency, if the incoming is other allele (default) or effect allele (use the --allelefreqswitch). Apparently it did not have any visible effect on the values at a first glance, but the switch will be in use for future formulas and if there actually are real differences for the present formulas.


## [1.0.0] - 2021-01-18
### Added
- A singularity .def file to show how to build the software as a singularity image.
- README with all instructions to build, check version and perform some internal tests.
- A working version implementaton of r stat functions using the unix-stream philosophy.

