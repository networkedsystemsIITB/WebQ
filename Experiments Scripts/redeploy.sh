#!/bin/sh

#run from proxy1 server(same as capacity estimator)
marker="=================================="
tokencheck="10.129.41.67"
tokengen="10.129.26.130"
tokengen2="10.129.41.17"
server="10.129.49.76"
vachaspati="10.129.2.55"
server2="comp4"
if [ -z "$1" -o -z "$2" ];
then
    echo "input username ./redeploy.sh <username> <port>"
    exit
else
    username=$1
    port=$2
    echo "username : ${username}"
    echo "port : ${port}"
fi

printf "%d %s%40s\n" $? $marker "Killing all components"
for machine in $tokengen $tokengen2
do
    ssh root@$machine "killall java &> /dev/null"
    ssh root@$machine "killall apache2 &> /dev/null"
done
ssh root@$tokencheck "killall lighttpd &> /dev/null";

#stop server, make the proxy1 code, and copy it in /usr/lib/cgi-bin
#then start the server
if [ "$2" = "moodle" ];
then
printf "%d %s%40s\n" $? $marker "remaking proxy1 -> moodle"
sshpass -p "webq" ssh root@10.129.26.130 "cd /home/${username}/webq/TokenGenNew; ./make_script.sh moodle"
else
printf "%d %s%40s\n" $? $marker "remade proxy1 -> php"
sshpass -p "webq" ssh root@10.129.26.130 "cd /home/${username}/webq/TokenGenNew; ./make_script.sh"
fi

#cleaning up all the log files
# echo "Cleaning up the log files"
# for machine in $tokengen $tokengen2
# do
#     ssh root@$machine "cat /dev/null > /home/${username}/webq/TokenGenNew/proxy1.log"
#     ssh root@$machine "cat /dev/null > /home/${uesrname}/webq/CapacityEstimator/javapersecond.log"
#     ssh root@$machine "cat /dev/null > /home/${username}/webq/CapacityEstimator/javadebug.log"
# done
# ssh webq@$vachaspati rm /home/webq/summary60.csv &> /dev/null
# ssh webq@$vachaspati rm /home/webq/summary110.csv &> /dev/null

for machine in $tokengen $tokengen2
do
    printf "%d %s%40s\n" $? $marker "start the apache server at $machine"
    ssh root@$machine "service apache2 start &> /dev/null"
    #hit the URL once
    printf "%d %s%40s\n" $? $marker "Hitting the URL once `grep $machine /etc/hosts`"
    lynx -dump http://$machine:${port}/proxy1\?limit\=100 > /dev/null;
    # sleep 5;
    #start java code
    printf "%d %s%40s\n" $? $marker "Starting the java code"
    ssh root@$machine "cd /home/${username}/webq/CapacityEstimator;bash run.sh;"
done

#start lighttpd
printf "%d %s%40s\n" $? $marker "Starting the lighttpd server"
ssh root@$tokencheck "bash /home/${username}/webq/TokenCheck/run.sh murali"

# echo "Setting $server and $server2 governor to performance:"
# for i in `seq 0 3`; do
# 	ssh root@$server "cpufreq-set -g performance -c $i";
# 	ssh root@$server2 "cpufreq-set -g performance -c $i";
# done;
# echo "Checking if governor is set correctly:"
# ssh root@$server "cpufreq-info | grep \"governor \"";
# ssh root@$server2 "cpufreq-info | grep \"governor \"";

echo "################# REDEPLOYMENT ATTEMPT FINISHED ##################";

echo "$tokengen:$port/proxy1?limit=100"
echo "$tokengen2:$port/proxy1?limit=100"
