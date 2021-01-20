# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Released]

## [1.0.1] - 2021-01-20
### Added

- Switch for allele frequency, if the incoming is other allele (default) or effect allele (use the --allelefreqswitch). Apparently it did not have any visible effect on the values at a first glance, but the switch will be in use for future formulas and if there actually are real differences for the present formulas.


## [1.0.0] - 2021-01-18
### Added

- A singularity .def file to show how to build the software as a singularity image.
- README with all instructions to build, check version and perform some internal tests.
- A working version implementaton of r stat functions using the unix-stream philosophy.

