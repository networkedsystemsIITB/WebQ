#!/bin/bash
make
sudo cp proxy1 /usr/lib/cgi-bin/proxy1
sudo cp proxy.conf /usr/lib/cgi-bin/proxy.conf
echo "copied proxy1"
