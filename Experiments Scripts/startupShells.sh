
TERM='urxvt'
#easy shortcuts
#$TERM -e ssh webq@10.129.41.8 &    #gen
$TERM -e ssh webq@10.129.41.67 &    #check
$TERM -e ssh webq@10.129.49.76 &    #server
$TERM -e ssh webq@10.129.2.55 &     #laod gen



#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.8  'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.41.67 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.49.76 'cat >> .ssh/authorized_keys'
#cat ~/.ssh/id_rsa.pub |ssh webq@10.129.2.55  'cat >> .ssh/authorized_keys'
