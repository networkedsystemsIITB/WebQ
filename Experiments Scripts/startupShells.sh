#!/bin/bash

TERM='urxvt'
#easy shortcuts
#$TERM -g +0+0 -e ssh webq@10.129.41.8 &    #gen
$TERM -g +0+400 -e ssh webq@10.129.26.130 &     #gen
$TERM -g +800+0 -e ssh webq@10.129.41.67 &    #check
$TERM -g +800+400 -e ssh webq@10.129.49.76 &    #server
$TERM -g +0+400 -e ssh webq@10.129.2.55 &     #laod gen



#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.8  'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.26.130  'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.67 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.49.76 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.2.55  'cat >> .ssh/authorized_keys'
