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

# get command line arguments {{{
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
# }}}

#stop server, make the proxy1 code, and copy it in /usr/lib/cgi-bin
#then start the server

#{{{ kill all components
printf " %d\n%s%43s" $? $marker "Killing all components" | tee -a $log_file
for machine in $tokengen $tokengen2
do
    ssh root@$machine "killall java"
    ssh root@$machine "killall apache2"
done
ssh root@$tokencheck "killall lighttpd";
#}}}

# cleaning up all the log files#{{{
for machine in $tokengen $tokengen2
do
    printf " %d\n%s%43s" $? $marker "Cleaning up the log files at $machine" | tee -a $log_file
    ssh root@$machine "cat /dev/null > /home/${username}/webq/TokenGenNew/proxy1.log"
#     ssh root@$machine "cat /dev/null > /home/${uesrname}/webq/CapacityEstimator/javapersecond.log"
#     ssh root@$machine "cat /dev/null > /home/${username}/webq/CapacityEstimator/javadebug.log"
done
# ssh webq@$vachaspati rm /home/webq/summary60.csv &> $log_file
# ssh webq@$vachaspati rm /home/webq/summary110.csv &> $log_file
#}}}

# rebuild and copy tokengen(proxy1) to cgi-bin folder#{{{
for machine in $tokengen $tokengen2
do
    printf " %d\n%s%43s" $? $marker "remade proxy1 -> php @ $machine" | tee -a $log_file
    sshpass -p "webq" ssh root@$machine \
        "cd /home/${username}/webq/TokenGenNew; ./make_script.sh" >> $log_file
done
#}}}

#{{{
for machine in $tokengen $tokengen2
do
    printf " %d\n%s%43s" $? $marker "start the apache server at $machine" | tee -a $log_file
    ssh root@$machine "service apache2 start" >> $log_file
    # #hit the URL once
    printf " %d\n%s%43s" $? $marker "Hitting the URL once `grep $machine /etc/hosts`" | tee -a $log_file
    lynx -dump http://$machine:${port}/proxy1\?limit\=100 >> $log_file;
    # sleep 5;
    #start java code
    printf " %d\n%s%43s" $? $marker "Starting the java code" | tee -a $log_file
    ssh root@$machine "cd /home/${username}/webq/CapacityEstimator;bash run.sh;"
done
#}}}

######start lighttpd
# printf " %d\n%s%46s" $? $marker "Starting the lighttpd server" | tee -a $log_file
# ssh root@$tokencheck "bash /home/${username}/webq/TokenCheck/run.sh murali"

# echo "################# REDEPLOYMENT ATTEMPT FINISHED ##################";

# echo "$tokengen:$port/proxy1?limit=100"
# echo "$tokengen2:$port/proxy1?limit=100"
