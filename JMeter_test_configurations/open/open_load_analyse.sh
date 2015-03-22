#!/bin/bash

noOfSeconds=10

for threads in {1..200..10}
do
    awk -F'[:,]' '{if(NR>1){print $1*60*60+$2*60+$3+$4*10^-3}}' march14-$threads.csv | sort -n |\
        sed '1p;$p;d' | awk '{ if(NR==1)old=$1;} END{print '$threads'" "('$threads'*'$noOfSeconds')/($1 - old)}' >> throughput.dat
    sed '1d;p' march14-$threads.csv | awk -F, '{sum+=$2}END{ print '$threads'" "sum/NR}' >> responsetime.dat
done

gnuplot graph.gnu
