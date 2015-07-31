#!/bin/bash

if [ -z "$1" ];
then
    echo "pass user account. eg ~/dev_setup <username>"
    exit
fi

echo 'alias jl="vim ~/webq/journal/`date --iso`"' >> ~/.localaliases.sh

source ~/webq/bin/ips.sh

echo "$server           server" | sudo tee -a /etc/hosts
echo "${tokencheck[$1]} check" | sudo tee -a /etc/hosts
echo "${tokengen1[$1]}  gen1" | sudo tee -a /etc/hosts
echo "${tokengen2[$1]}  gen2" | sudo tee -a /etc/hosts
echo "$vachaspati       vacha" | sudo tee -a /etc/hosts

cd ~/webq
git update-index --assume-unchanged TokenCheck/lighttpd/src/*.o
git update-index --assume-unchanged TokenCheck/lighttpd/src/.libs/**
git update-index --assume-unchanged TokenCheck/lighttpd/**/Makefile
git update-index --assume-unchanged CapacityEstimator/*.log
git update-index --assume-unchanged TokenGenNew/proxy1
git update-index --assume-unchanged TokenCheck/lighttpd/config.h
git update-index --assume-unchanged TokenCheck/lighttpd/config.log
git update-index --assume-unchanged TokenCheck/lighttpd/config.status
git update-index --assume-unchanged TokenCheck/lighttpd/libtool
git update-index --assume-unchanged TokenCheck/lighttpd/src/lighttpd
git update-index --assume-unchanged TokenCheck/lighttpd/src/proc_open
git update-index --assume-unchanged TokenCheck/lighttpd/src/spawn-fcgi
git update-index --assume-unchanged TokenGenNew/waittimedebug.log
git update-index --assume-unchanged TokenGenNew/proxy1
git update-index --assume-unchanged TokenGenNew/proxy1.log
git update-index --assume-unchanged CapacityEstimator/bin/**/*.class

sudo apt-get install sshpass lynx
