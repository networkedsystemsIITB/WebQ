#!/bin/bash
# some binary files have been inadventantly commited into git tree .
# use this script to prevent them from bothering you

git update-index --assume-unchanged \
CapacityEstimator/bin/com/webq/capest/*.class
