#!/bin/bash

curl --data "macid=00:22:4d:b2:12:7b" 'http://cuse.iitb.ac.in/wol/wol.php' #49.76 server
curl --data "macid=00:22:4d:b1:fc:a9" 'http://cuse.iitb.ac.in/wol/wol.php' #41.67 check
curl --data "macid=7c:05:07:10:41:2f" 'http://cuse.iitb.ac.in/wol/wol.php' #26.130 gen1
sleep 2
