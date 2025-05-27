# Testing Examples and Usage Guide

This document contains comprehensive testing examples and usage patterns for r-stats-c-streamer.

## Prerequisites

Before running these tests, make sure you have:
- Built the project using `./build.sh`
- Verified the installation with `./build/r-stats-c-streamer --version`

## Basic Testing

### Linear Regression Statistics

```bash
# Look at test data using head
cat test/testdata/linear_testStats.txt | head | column -t

# Create function file for linear regression
echo -e "zscore_from_pval_beta
zscore_from_pval_beta_N
zscore_from_beta_se
pval_from_zscore_N
pval_from_zscore
beta_from_zscore_se
beta_from_zscore_N_af
se_from_zscore_beta
se_from_zscore_N_af
N_from_zscore_beta_af" > functiontestfile.txt

# Test linear model
cat test/testdata/linear_testStats.txt | ./build/r-stats-c-streamer \
    --functionfile functiontestfile.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --beta 2 \
    --standarderror 3 \
    --Nindividuals 6 \
    --zscore 4 \
    --allelefreq 7 \
    --statmodel lin | head | column -t
```

Expected output:
```
0             zscore_from_pval_beta  zscore_from_pval_beta_N  zscore_from_beta_se
rs4819391_G   1.832718               1.834151                 1.834151
rs11089128_G  -0.808975              -0.809215                -0.809215
rs7288972_C   -1.075487              -1.075903                -1.075903
rs2032141_A   2.068237               2.070195                 2.070195
```

### Logistic Regression Statistics

```bash
# Create function file for logistic regression
echo -e "zscore_from_beta_se
zscore_from_pval_oddsratio
pval_from_zscore
beta_from_oddsratio
beta_from_zscore_se
se_from_beta_zscore
se_from_ORu95_ORl95
Neff_from_Nca_Nco" > functiontestfile_logistic.txt

# Test logistic model
cat test/testdata/logistic_testStats.txt | ./build/r-stats-c-streamer \
    --functionfile functiontestfile_logistic.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 9 \
    --beta 3 \
    --standarderror 4 \
    --Nindividuals 10 \
    --Ncases 11 \
    --Ncontrols 12 \
    --zscore 8 \
    --allelefreq 13 \
    --oddsratio 5 \
    --ORu95 6 \
    --ORl95 7 \
    --statmodel log
```

## Advanced Features

### Handling -log10 P-values

When your input data contains -log10 transformed p-values, use the `--neglog10p` flag:

```bash
# Test with -log10 p-values
cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer \
    --functionfile functiontestfile.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --beta 2 \
    --standarderror 3 \
    --Nindividuals 6 \
    --zscore 4 \
    --allelefreq 7 \
    --statmodel lin \
    --neglog10p | head | column -t
```

### Column Replacement Functionality

You can replace specific columns in your input data with calculated values:

```bash
# Create function file for p-value conversion
echo -e "pval_from_neglog10p" > functiontestfile2.txt

# Convert -log10 p-values to normal p-values (replace column 5)
cat test/testdata/linear_testStats_neglog10Pvalue.txt | ./build/r-stats-c-streamer \
    --functionfile functiontestfile2.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --statmodel none \
    --replace 5 | head | column -t
```

Expected output:
```
RSID          B                    SE                  Z                   neglog10P
rs4819391_G   0.233253438056324    0.127172429896691   1.83415098890387    1.17493e-02
rs11089128_G  -0.057230689617411   0.0707237246183849  -0.809214869921227  3.78274e-01
rs7288972_C   -0.0603896529952508  0.0561292812495112  -1.07590283807129   5.49510e-01
```

### Effect Allele Frequency from Case/Control Data

For logistic regression with case/control allele frequencies:

```bash
# Look at case/control AF test data
cat test/testdata/logistic_testStats_caseAF_controlAF.txt | head | column -t

# Create function file for EAF calculation
echo -e "zscore_from_beta_se
AF_from_CaseAF_ControlAF" > functiontestfile_logistic_cacoaf.txt

# Calculate overall EAF from case/control AFs
cat test/testdata/logistic_testStats_caseAF_controlAF.txt | ./build/r-stats-c-streamer \
    --functionfile functiontestfile_logistic_cacoaf.txt \
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
    --statmodel log | head | column -t
```

## Automated Testing

### Complete Test Suite

Run the comprehensive unit test framework:

```bash
# Run all tests from project root
./scripts/test.sh
```

This will automatically:
- Build the project
- Run all unit tests
- Report detailed results with pass/fail status
- Provide a summary of test outcomes

### Individual Unit Tests

You can also run specific tests:

```bash
# Test basic linear regression functionality
./test/units/test_linear_basic.sh

# Test precision fixes
./test/units/test_precision_fix.sh

# Test column replacement
./test/units/test_column_replacement.sh
```

See `test/units/README.md` for complete documentation of the unit test framework.

## Precision Testing

### High-Precision P-value Conversion

The precision fix is automatically tested as part of the unit test suite, but you can also run it manually:

```bash
# Run the comprehensive precision test
cd test/neglog10p_precision_fix
./run_test.sh
cd ../..
```

This test verifies that highly significant p-values (e.g., -log10 p > 6) are converted correctly instead of being truncated to 0.000000. See `docs/test_neglog10p_fix.md` for detailed information about this bug fix.

## Singularity Container

### Building the Container

```bash
# Make singularity image based on definition file
mkdir -p images
fname="$(date +%F)"-ubuntu-2004_r-stats-c-streamer.simg
sudo singularity build images/${fname} ubuntu-20.04_r-stats-c-streamer.def
```

### Testing the Container

```bash
# Test the singularity container (adjust date as needed)
cat test/testdata/linear_testStats.txt | singularity run --bind .:/mnt \
    images/2020-12-14-ubuntu-2004_r-stats-c-streamer.simg \
    r-stats-c-streamer \
    --functionfile /mnt/functiontestfile.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 5 \
    --beta 2 \
    --standarderror 3 \
    --Nindividuals 6 \
    --zscore 4 \
    --allelefreq 7 \
    --statmodel lin | head | column -t
```

## Available Functions

### Linear Regression Functions
- `zscore_from_pval_beta`
- `zscore_from_pval_beta_N`
- `zscore_from_beta_se`
- `pval_from_zscore_N`
- `pval_from_zscore`
- `beta_from_zscore_se`
- `beta_from_zscore_N_af`
- `se_from_zscore_beta`
- `se_from_zscore_N_af`
- `N_from_zscore_beta_af`

### Logistic Regression Functions
- `zscore_from_beta_se`
- `zscore_from_pval_oddsratio`
- `pval_from_zscore`
- `beta_from_oddsratio`
- `beta_from_zscore_se`
- `se_from_beta_zscore`
- `se_from_ORu95_ORl95`
- `Neff_from_Nca_Nco`
- `AF_from_CaseAF_ControlAF`

### Utility Functions
- `pval_from_neglog10p`
- `log10_to_p`

## Command Line Options

| Option | Short | Description |
|--------|-------|-------------|
| `--version` | `-v` | Show version information |
| `--skiplines` | `-s` | Number of header lines to skip |
| `--index` | `-i` | Column number for variant ID |
| `--pvalue` | `-p` | Column number for p-value |
| `--beta` | `-b` | Column number for beta/effect size |
| `--standarderror` | `-e` | Column number for standard error |
| `--Nindividuals` | `-n` | Column number for sample size |
| `--zscore` | `-z` | Column number for z-score |
| `--allelefreq` | `-a` | Column number for allele frequency |
| `--oddsratio` | `-o` | Column number for odds ratio |
| `--Ncases` | | Column number for number of cases |
| `--Ncontrols` | | Column number for number of controls |
| `--CaseAF` | `-y` | Column number for case allele frequency |
| `--ControlAF` | `-t` | Column number for control allele frequency |
| `--functionfile` | `-f` | File containing functions to apply |
| `--statmodel` | `-m` | Statistical model (lin/log/none) |
| `--neglog10p` | | Input p-values are in -log10 scale |
| `--allelefreqswitch` | `-w` | Switch allele frequency interpretation |
| `--replace` | | Replace specified column with calculated value |

## Troubleshooting

### Common Issues

1. **Build failures**: Ensure cmake and rmath library are installed
2. **Precision issues**: Use the latest version (1.4.0+) for high-precision p-value calculations
3. **Column indexing**: Remember that column numbers are 1-indexed, not 0-indexed
4. **Function file format**: Each function name should be on a separate line

### Getting Help

- Check the version: `./build/r-stats-c-streamer --version`
- See developer documentation: `docs/developers.md`
- Review changelog: `CHANGELOG.md` 