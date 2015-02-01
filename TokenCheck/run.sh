#!/bin/bash

cd /home/webq/webq-repo/TokenCheck/lighttpd;
sudo lighttpd -f /home/webq/webq-repo/TokenCheck/lighttpd/lighttpd.conf;
echo "successfully started lighttpd"
