#!/bin/bash
marker="=================================="
server="10.129.49.76"
vachaspati="10.129.2.55"
log_file=large_log.log


declare -A tokencheck
tokencheck[stanly]="10.129.41.17"
tokencheck[murali]="10.129.41.67"
tokencheck[ravi]="10.129.26.133"

declare -A tokengen1
tokengen1[stanly]="10.129.26.133"
tokengen1[murali]="10.129.28.32"
tokengen1[ravi]="10.129.41.67"

declare -A capacityEstimator
capacityEstimator[stanly]="10.129.26.133"
capacityEstimator[murali]="10.129.41.17"
capacityEstimator[ravi]="10.129.41.67"

declare -A tokengen2
tokengen2[stanly]=""
tokengen2[murali]="10.129.41.17"
tokengen2[ravi]=""

#                          |murali   |  stan      ravi
#--------------------------|---------|------------------
#      | 10.129.41.17      | gen2    |  check
#      | 10.129.41.67      | check   |            gen1
#      | 10.129.26.133     |         |  gen1      check
#      | 10.129.2.55       | vacha   |  vacha     vacha
#      | 10.129.49.76      |         |  server
# stan | 10.129.28.32      | gen1    |
# mura | 10.129.28.146     | server  |
