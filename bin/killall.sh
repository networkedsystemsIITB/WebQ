#!/bin/sh

#run from proxy1 server(same as capacity estimator)
marker="=================================="
tokencheck="10.129.41.67"
# tokengen="10.129.26.130"
tokengen2="10.129.41.17"
server="10.129.49.76"
vachaspati="10.129.2.55"
server2="comp4"
log_file=large_log.log


#{{{ kill all components
printf " %d\n%s%43s" $? $marker "Killing all components" | tee -a $log_file
for machine in $tokengen $tokengen2
do
    ssh root@$machine "killall java"
    ssh root@$machine "killall apache2"
done
ssh root@$tokencheck "killall lighttpd";
#}}}
