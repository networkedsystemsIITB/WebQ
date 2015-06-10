#!/bin/bash
if [ -z "$1" ];
then
    echo "pass user account. eg ~/dev_setup <username>"
    exit
fi

cat ~/.ssh/id_rsa.pub |ssh ${1}@10.129.41.8  'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@10.129.26.130  'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@10.129.41.67 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@10.129.49.76 'cat >> .ssh/authorized_keys'
cat ~/.ssh/id_rsa.pub |ssh ${1}@10.129.2.55  'cat >> .ssh/authorized_keys'
