#!/bin/bash
printf "\ngonna build java in dir :`pwd`"
nohup java -cp jars/commons-math3-3.3.jar:jars/log4j-1.2.14.jar:jars/matlabcontrol-4.1.0.jar:bin com.webq.capest.CapacityManager > foo.out 2> foo.err < /dev/null &
