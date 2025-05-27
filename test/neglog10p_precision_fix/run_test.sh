#!/bin/bash

# Test script for neglog10p precision fix
# This script tests that highly significant p-values are converted correctly
# instead of being truncated to 0.000000

echo "Testing neglog10p precision fix..."
echo "=================================="

# Run the test
cat test_data.txt | ../../build/r-stats-c-streamer \
    --functionfile test_neglog10p_precision.txt \
    --skiplines 1 \
    --index 1 \
    --pvalue 2 \
    --statmodel none

echo ""
echo "Expected results:"
echo "rs_high_sig_1: ~5.03e-12 (not 0.000000)"
echo "rs_high_sig_2: ~7.21e-08 (not 0.000000)" 
echo "rs_high_sig_3: ~6.17e-08 (not 0.000000)"
echo "rs_medium_sig: ~1.00e-05"
echo "rs_low_sig: ~1.00e-02"
echo "rs_normal: ~1.00e-01" 