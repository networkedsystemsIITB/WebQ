#!/bin/bash
# use this script to enable passwordless entry into another account
# like to setup passwordless entry into root account of all the comps 
# ./makeKnownHost.sh root

echo "dev setup should be run before runnign this file"

if [[ ! -f ~/.ssh/id_rsa.pub ]];
then
    ssh-keygen -t rsa
fi

ssh server 'mkdir -p ~/.ssh'
ssh gen 'mkdir -p ~/.ssh'
ssh gen2 'mkdir -p ~/.ssh'
ssh check 'mkdir -p ~/.ssh'
ssh vacha 'mkdir -p ~/.ssh'
cat ~/.ssh/id_rsa.pub |ssh server  'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh gen 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh gen2 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh check 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh vacha  'cat >> .ssh/authorized_keys'
