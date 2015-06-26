#!/bin/bash

cd ~/webq/bin
source ips.sh

#{{{ kill all components
printf " %d\n%s%43s\n" $? $marker "Killing all components" | tee -a $log_file
for machine in $tokengen1 $tokengen2
do
    ssh root@$machine "killall java"
    ssh root@$machine "killall apache2"
done
ssh root@$tokencheck "killall lighttpd";
#}}}
