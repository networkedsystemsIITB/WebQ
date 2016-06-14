#!/bin/bash
marker="=================================="
server="10.129.49.76"
vachaspati="10.129.2.55"
log_file=large_log.log


tokencheck="10.129.26.133"
tokengen1="10.129.28.32"
capacityEstimator="10.129.28.160"
tokengen2="10.129.28.160"

#                           |               | murali             | srishti    | paresh
#-------------------------- | --------      | ------------------ | ---------- |
#                           | 10.129.28.160 | gen2&ce            | check      | gen&ce
#                           | 10.129.41.67  |                    |            |
#                           | 10.129.26.133 | check ( spare1 )   | server     |
#                           | 10.129.49.76  | server( spare2 )   | gen&ce     | check
# vachaspati                | 10.129.2.55   | vacha              | vacha      | vacha
# stan mtp                  | 10.129.28.32  |                    |            | server
# muru mtp                  | 10.129.28.146 |                    |
