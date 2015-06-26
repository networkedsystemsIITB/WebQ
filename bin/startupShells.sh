#!/bin/bash

TERM='urxvt'
#easy shortcuts
#$TERM -g +0+0 -e ssh webq@10.129.41.8 &    #gen2
$TERM -g +0+400 -e ssh webq@10.129.26.130 &     #gen1
$TERM -g +800+0 -e ssh webq@10.129.41.67 &    #check
$TERM -g +800+400 -e ssh webq@10.129.49.76 &    #server
$TERM -g +0+0 -e ssh -X webq@10.129.2.55 &     #loadGenerator



