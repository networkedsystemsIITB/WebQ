#!/bin/bash
# use this script to enable passwordless entry into another account
# like to setup passwordless entry into root account of all the comps 
# ./makeKnownHost.sh root <ip>

# echo "dev setup should be run before runnign this file"

if [ -z "$1" -o  -z "$2" ];
then
    echo "input username ./redeploy.sh <username> <hostnam/ip>"
    exit
else
    USER=$1
    IP=$2
fi

if [[ ! -f ~/.ssh/id_rsa.pub ]];
then
    ssh-keygen -t rsa
fi


ssh ${USER}@${IP} 'mkdir -p ~/.ssh'
cat ~/.ssh/id_rsa.pub |ssh ${USER}@${IP} 'cat >> .ssh/authorized_keys'
