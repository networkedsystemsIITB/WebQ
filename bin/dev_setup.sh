#!/bin/bash

if [ -z "$1" ];
then
    echo "pass user account. eg ~/dev_setup <username>"
    exit
fi

echo 'alias jl="vim ~/webq/journal/`date --iso`"' >> ~/.localaliases.sh

echo "10.129.49.76    server" | sudo tee -a /etc/hosts
echo "10.129.41.67    check" | sudo tee -a /etc/hosts
echo "10.129.26.130   gen" | sudo tee -a /etc/hosts
echo "10.129.41.17   gen2" | sudo tee -a /etc/hosts
echo "10.129.2.55 vacha" | sudo tee -a /etc/hosts

cat << EOF >> ~/.ssh/config
Host server
	HostName 10.129.49.76
	User	$1

Host check
	HostName 10.129.41.67
	User	$1

Host gen
	HostName 10.129.26.130
	User	$1

Host gen2
	HostName 10.129.41.17
	User	$1

Host vacha
	HostName 10.129.2.55
	User	$1
EOF

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
git update-index --assume-unchanged TokenGenNew/proxy1.log

sudo apt-get install sshpass lynx
