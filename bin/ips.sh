#!/bin/bash
marker="=================================="
server="10.129.49.76"
vachaspati="10.129.2.55"
log_file=large_log.log


tokencheck="10.129.26.133"
tokengen1="10.129.28.32"
capacityEstimator="10.129.28.160"
tokengen2="10.129.28.160"

#                           |               | murali             | srishti
#-------------------------- | --------      | ------------------ | ----------
#                           | 10.129.28.160 | gen2&ce            | gen2&ce
#                           | 10.129.41.67  |                    |
#                           | 10.129.26.133 | check ( spare1 )   | check
#                           | 10.129.49.76  | server( spare2 )   | server
# vachaspati                | 10.129.2.55   | vacha              | vacha
# stan mtp                  | 10.129.28.32  |                    |
# muru mtp                  | 10.129.28.146 |                    |
