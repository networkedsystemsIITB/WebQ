#!/bin/bash


curl 'cuse.iitb.ac.in/wol/wol.php?mac=00:22:4d:b2:12:7b'        #49.76 server
curl 'cuse.iitb.ac.in/wol/wol.php?mac=00:22:4d:b1:fc:a9'        #41.67 check
curl 'cuse.iitb.ac.in/wol/wol.php?mac=7c:05:07:10:41:2f'        #26.130 gen

TERM='urxvt'
#easy shortcuts
#$TERM -g +0+0 -e ssh webq@10.129.41.8 &    #gen
$TERM -g +0+400 -e ssh webq@10.129.26.130 &     #gen
$TERM -g +800+0 -e ssh webq@10.129.41.67 &    #check
 $TERM -g +800+400 -e ssh webq@10.129.49.76 &    #server
$TERM -g +0+0 -e ssh -X webq@10.129.2.55 &     #laod gen



#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.8  'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.26.130  'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.67 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.49.76 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.2.55  'cat >> .ssh/authorized_keys'
