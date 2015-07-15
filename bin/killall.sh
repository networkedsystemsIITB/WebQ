#!/bin/bash


if [ -z "$1" ];
then
    echo "input username ./killall.sh <username>"
    exit
fi

cd ~/webq/bin
source ips.sh
printf "`date`" | tee -a $log_file

#{{{ kill all components
printf " %d\n%s%43s\n" $? $marker "Killing all components" | tee -a $log_file
if [ -z "$gens" ];
then
    gens="${tokengen2[$1]} ${tokengen1[$1]}"
fi
echo "in kilall, gens considered are: $gens"
for machine in $gens
do
    # redirect stderr to stdout so that we can log it 
    ssh root@$machine "killall java" 2>&1 | tee -a $log_file > /dev/null
    ssh root@$machine "killall apache2" 2>&1 | tee -a $log_file > /dev/null
done
ssh root@${tokencheck[$1]} "killall lighttpd" 2>&1  | tee -a $log_file > /dev/null
#}}}
