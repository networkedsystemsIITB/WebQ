#!/bin/bash
make

if [ "$1" == "moodle" ];
then
    sudo cp proxy1_moodle /usr/lib/cgi-bin/proxy1
else
    sudo cp proxy1 /usr/lib/cgi-bin/proxy1
fi
