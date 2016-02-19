#!/bin/bash
marker="=================================="
server="10.129.49.76"
vachaspati="10.129.2.55"
log_file=large_log.log


declare -A tokencheck
tokencheck[stanly]="10.129.28.160"
tokencheck[murali]="10.129.41.67"
tokencheck[ravi]="10.129.26.133"

declare -A tokengen1
tokengen1[stanly]="10.129.26.133"
tokengen1[murali]="10.129.28.32"
tokengen1[ravi]="10.129.41.67"

declare -A capacityEstimator
capacityEstimator[stanly]="10.129.26.133"
capacityEstimator[murali]="10.129.28.160"
capacityEstimator[ravi]="10.129.41.67"

declare -A tokengen2
tokengen2[stanly]=""
tokengen2[murali]="10.129.28.160"
tokengen2[ravi]=""

#                          |murali   |  matlab
#--------------------------|---------|---------
#      | 10.129.28.160     | gen2    |
#      | 10.129.41.67      | check   |
#      | 10.129.26.133     | sapre1  |  gen1
#      | 10.129.2.55       | vacha   |  vacha
#      | 10.129.49.76      |         |  server
# stan | 10.129.28.32      | gen1    |  check
# mura | 10.129.28.146     | server  |
# aaksh| 10.129.28.110
