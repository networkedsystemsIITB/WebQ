#!/bin/bash

#run from proxy1 server(same as capacity estimator)
cd ~/webq/bin
source ~/webq/bin/ips.sh

# get command line arguments {{{
if [ -z "$1" ];
then
    echo "input username ./redeploy.sh <username> <no_of_tokengen>"
    exit
else
    username=$1
    echo "username : ${username}"
fi
# }}}

# {{{ put the number of tokengens
if [ -z "$2" ];
then
    gens="${tokengen1[$username]}"
else
    case $2 in
        1)
            gens="${tokengen2[$username]}"
            ;;
        2)
            gens="${tokengen2[$username]} ${tokengen1[$username]}"
            ;;
        *)
            gens="${tokengen2[$username]}"
            ;;
    esac
fi
echo "gens = $gens"
export gens
# }}}

# The following is done in the following code
# stop server, make the proxy1 code, and copy it in /usr/lib/cgi-bin
# then start the server

# killall.sh will stop all tokenGens and TokenChecks
bash ~/webq/bin/killall.sh $username                  #kill all components 

# cleaning up all the log files#{{{
for machine in $gens
do
    printf " %d\n%s%43s" $? $marker "Cleaning up the log files at $machine" | tee -a $log_file
    ssh root@$machine "cat /dev/null > /home/${username}/webq/TokenGenNew/proxy1.log"
    ssh root@$machine "cat /dev/null > /usr/lib/cgi-bin/proxy1.log"
#     ssh root@$machine "cat /dev/null > /home/${uesrname}/webq/CapacityEstimator/javapersecond.log"
#     ssh root@$machine "cat /dev/null > /home/${username}/webq/CapacityEstimator/javadebug.log"
done
# ssh webq@$vachaspati rm /home/webq/summary60.csv &> $log_file
# ssh webq@$vachaspati rm /home/webq/summary110.csv &> $log_file
#}}}

# rebuild and copy tokengen(proxy1) to cgi-bin folder#{{{
for machine in $gens
do
    printf " %d\n%s%43s" $? $marker "remade proxy1 -> php @ $machine" | tee -a $log_file
    sshpass -p "webq" ssh root@$machine \
        "cd /home/${username}/webq/TokenGenNew; ./make_script.sh" >> $log_file
done
#}}}

# {{{  now wait for some time 
# as apache need some time to recover !! :P
# if "SOME" thrid parameter is passed do not wait
if [ -z "$3" ];
then
    # empty statement | do nothing
    :
else
    # unset
    printf " %d\n%s%43s" $? $marker "wait for some time for apache to recover" | tee -a $log_file
    ttw=40  #time to wait
    printf "\n"
    for i in `seq $ttw -1 1`
    do
        printf "%3d" $i
        sleep 1
        if (( i%10 == 0)) ; then
            printf "\n"
        fi
    done
    printf "\n"
fi
# }}}

#{{{ start apache and hence the proxy and hit url
for machine in $gens
do
    printf " %d\n%s%43s" $? $marker "start the apache server at $machine" | tee -a $log_file
    ssh root@$machine "service apache2 start" >> $log_file
done
# we need a separate for loop here so that we can hit the machines 
# in parellel at the same time 
for machine in $gens
do
    # #hit the URL once
    printf " %d\n%s%43s" $? $marker "Hitting the URL once `grep $machine /etc/hosts`" | tee -a $log_file
    lynx -dump http://$machine:8000/proxy1\?limit\=100 >> $log_file &
done
#}}}

#start java code {{{
printf " %d\n%s%43s" $? $marker "Starting the java code" | tee -a $log_file
ssh root@${capacityEstimator[$username]} "cd /home/${username}/webq/CapacityEstimator;bash Makefile"
ssh root@${capacityEstimator[$username]} "cd /home/${username}/webq/CapacityEstimator;bash run.sh;"

#}}}

# {{{ start lighttpd
sleep 4
printf " %d\n%s%46s\n" $? $marker "Starting the lighttpd server" | tee -a $log_file
ssh root@${tokencheck[$username]} "bash /home/${username}/webq/TokenCheck/run.sh ${username}"

echo "################# REDEPLOYMENT ATTEMPT FINISHED ##################";

echo "$tokengen1:8000/proxy1?limit=100"
echo "$tokengen2:8000/proxy1?limit=100"

# }}}
