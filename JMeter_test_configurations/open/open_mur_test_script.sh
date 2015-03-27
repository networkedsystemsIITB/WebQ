#!/bin/bash

noOfSeconds=10
if [ -z "$1"  ]; then
    # variable not passed
    echo "usage: pass password as argument "
else
    for threadno in {1..60..2}
    do
        sed -i 's/threads">\([0-9]\+\)/threads">'$(( $threadno * $noOfSeconds  ))'/' open_tstMarch09.jmx
        sed -i 's/14-\([0-9]\+\)/14-'$threadno'/' open_tstMarch09.jmx
        git --no-pager diff open_tstMarch09.jmx
        jmeter -n -t open_tstMarch09.jmx

        # analyse and grep/awk the output file

        awk -F'[:,]' '{if(NR>1){print $1*60*60+$2*60+$3+$4*10^-3}}' march14-$threadno.csv | sort -n |\
            sed '1p;$p;d' | awk '{ if(NR==1)old=$1;} END{print '$threadno'" "('$threadno'*'$noOfSeconds')/($1 - old)}' >> throughput.dat
        sed '1d;p' march14-$threadno.csv | awk -F, '{sum+=$2}END{ print '$threadno'" "sum/NR}' >> responsetime.dat
    done
    gnuplot graph.gnu
    # alram on my machine when all this is over !! ;)
    sshpass -p "$1" ssh murali@10.16.35.52 'paplay /usr/share/sounds/freedesktop/stereo/complete.oga'
fi

