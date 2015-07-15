#!/bin/bash
marker="=================================="
server="10.129.49.76"
vachaspati="10.129.2.55"
log_file=large_log.log


declare -A tokencheck
tokencheck[stanly]="10.129.41.17"
tokencheck[murali]="10.129.41.67"

declare -A tokengen1
tokengen1[stanly]="10.129.26.133"
tokengen1[murali]="10.129.26.133"

declare -A capacityEstimator
capacityEstimator[stanly]="10.129.26.133"
capacityEstimator[murali]="10.129.41.17"

declare -A tokengen2
tokengen2[stanly]=""
tokengen2[murali]="10.129.41.17"
