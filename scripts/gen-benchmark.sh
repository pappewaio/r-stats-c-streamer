#!/usr/bin/env bash

set -euo pipefail

awk -v n=1000000 -v seed="$RANDOM" 'BEGIN { srand(seed); for (i=0; i<n; ++i) printf("%.4f\n", rand()) }'
