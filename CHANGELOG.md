# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.4.0] - 2024-12-30
### Fixed
- Fixed precision issue in p-value conversion from -log10 scale where highly significant p-values (e.g., -log10 p > 6) were being truncated to 0.000000
- Updated printf format specifiers in p-value output functions to use scientific notation with higher precision (%.15e)
- Fixed `pval_from_neglog10p`, `pval_from_zscore`, `pval_from_zscore_N`, and logistic `pval_from_zscore` functions

### Added
- Comprehensive test suite for neglog10p precision fix in `test/neglog10p_precision_fix/`
- Documentation of the bug fix in `docs/test_neglog10p_fix.md`

## [1.3.0] - 2024-XX-XX
### Added
- (Note: This version's changes need to be documented)

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

