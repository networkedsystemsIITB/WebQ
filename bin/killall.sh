#!/bin/bash

cd ~/webq/bin
source ips.sh

#{{{ kill all components
printf " %d\n%s%43s\n" $? $marker "Killing all components" | tee -a $log_file
if [ -z "$gens" ];
then
    gens="$tokengen1 $tokengen2"
fi
echo "in kilall gens: $gens"
for machine in $gens
do
    ssh root@$machine "killall java"
    ssh root@$machine "killall apache2"
done
ssh root@$tokencheck "killall lighttpd";
#}}}
