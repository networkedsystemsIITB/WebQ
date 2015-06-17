#!/bin/sh

#run from proxy1 server(same as capacity estimator)
tokencheck="10.129.41.67"
tokengen="10.129.26.130"
server="10.129.49.76"
vachaspati="10.129.2.55"
server2="comp4"

#kill all the components
echo "Killing all components";
ssh root@$tokengen "killall java;"
# ps ax | grep CapacityManager | sed 's|  | |g' | cut -d' ' -f2 | xargs -I {} sudo kill -9 {}
ssh root@$tokencheck "killall lighttpd &> /dev/null";
ssh root@$tokengen "killall apache2;"

#stop server, make the proxy1 code, and copy it in /usr/lib/cgi-bin
#then start the server
echo "Remaking the proxy1";
if [ "$2" = "moodle" ];
then
echo "remade moodle"
sshpass -p "webq" ssh root@10.129.26.130 "cd /home/webq/webq-repo/TokenGenNew; ./make_script.sh moodle"
else
echo "remade php"
sshpass -p "webq" ssh root@10.129.26.130 "cd /home/webq/webq-repo/TokenGenNew; ./make_script.sh"
fi

#cleaning up all the log files
echo "Cleaning up the log files"
ssh root@$tokengen "cat /dev/null > /home/webq/webq-repo/TokenGenNew/proxy1.log"
ssh root@$tokengen "cat /dev/null > /home/webq/webq-repo/CapacityEstimator/javapersecond.log"
ssh root@$tokengen "cat /dev/null > /home/webq/webq-repo/CapacityEstimator/javadebug.log"
# ssh webq@$vachaspati rm /home/webq/summary60.csv &> /dev/null
# ssh webq@$vachaspati rm /home/webq/summary110.csv &> /dev/null

#start the apache server
ssh root@$tokengen "service apache2 start;"

#hit the URL once
echo "Hitting the URL once";
lynx -dump http://$tokengen:8080/proxy1\?limit\=100 > /dev/null;
sleep 5;

#start java code
echo "Starting the java code"
ssh root@$tokengen "cd /home/webq/webq-repo/CapacityEstimator;bash run.sh;"

#start lighttpd
echo "Starting the lighttpd server";
ssh root@$tokencheck "bash /home/webq/webq-repo/TokenCheck/run.sh"

# echo "Setting $server and $server2 governor to performance:"
# for i in `seq 0 3`; do
# 	ssh root@$server "cpufreq-set -g performance -c $i";
# 	ssh root@$server2 "cpufreq-set -g performance -c $i";
# done;
# echo "Checking if governor is set correctly:"
# ssh root@$server "cpufreq-info | grep \"governor \"";
# ssh root@$server2 "cpufreq-info | grep \"governor \"";

echo "################# REDEPLOYMENT ATTEMPT FINISHED ##################";

echo "$tokengen:8080/proxy1?limit=100"
