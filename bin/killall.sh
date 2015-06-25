#!/bin/sh

#run from proxy1 server(same as capacity estimator)
tokencheck="10.129.41.67"
# tokengen="10.129.26.130"
tokengen2="10.129.41.17"
server="10.129.49.76"
vachaspati="10.129.2.55"
server2="comp4"

if [ "$1" != "run2" ];
then
    echo "executed run.sh"
	tokencheckscript="run.sh"
else
    echo "executed run2.sh"
	tokencheckscript="run_twoservers.sh"
fi

#kill all the components
echo "Killing all components";
# ps ax | grep CapacityManager | sed 's|  | |g' | cut -d' ' -f2 | xargs -I {} sudo kill -9 {}
ssh root@$tokencheck "killall lighttpd &> /dev/null";
ssh root@$tokengen2 "killall java;"
ssh root@$tokengen2 "killall apache2;"

exit;


