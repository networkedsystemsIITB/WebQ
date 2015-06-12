#!/bin/bash

cd /home/webq/webq-repo/TokenCheck/lighttpd;
sudo lighttpd -f /home/webq/webq-repo/TokenCheck/lighttpd/lighttpd.conf;
if (( "$?" == 0 ));
then
    echo "SUCCESSFULLY STARTED LIGHTTPD"
else
    echo "ERROR STARTING LIGHTTPD"
fi
