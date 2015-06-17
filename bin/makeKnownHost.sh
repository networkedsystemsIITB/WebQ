#!/bin/bash
# use this script to enable passwordless entry into another account
# like to setup passwordless entry into root account of all the comps 
# ./makeKnownHost.sh root
if [ -z "$1" ];
then
    echo "pass user account. eg ~/dev_setup <username>"
    exit
fi

if [[ ! -f ~/.ssh/id_rsa.pub ]];
then
    ssh-keygen -t rsa
fi

ssh ${1}@server 'mkdir -p ~/.ssh'
ssh ${1}@gen 'mkdir -p ~/.ssh'
ssh ${1}@check 'mkdir -p ~/.ssh'
ssh ${1}@vacha 'mkdir -p ~/.ssh'
cat ~/.ssh/id_rsa.pub |ssh ${1}@server  'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@gen 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@check 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@vacha  'cat >> .ssh/authorized_keys'
