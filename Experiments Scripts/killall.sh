#!/bin/sh

tokencheck="wolverine2"
tokengen="comp4new"
server="comp1"

#kill all the components
echo "Killing all components";
ps ax | grep CapacityManager | sed 's|  | |g' | cut -d' ' -f2 | xargs -I {} sudo kill -9 {}
ssh root@$tokencheck "killall lighttpd &> /dev/null";
sudo service apache2 stop;

exit;


######################################################################################################3
#stop server, make the proxy1 code, and copy it in /usr/lib/cgi-bin
#then start the server
echo "Remaking the proxy1";
cd ../TokenGenNew;
make;

#cleaning up all the log files
echo "Cleaning up the log files"
sudo rm /home/webq/TokenGenNew/proxy1.log &> /dev/null
rm /home/webq/CapacityEstimator/javapersecond.log &> /dev/null
rm /home/webq/CapacityEstimator/javadebug.log &>/dev/null
ssh webq@vachaspati rm /home/webq/summary.csv &> /dev/null

#start the apache server
sudo service apache2 start;

#hit the URL once
echo "Hitting the URL once";
lynx -dump http://$tokengen/cgi-bin/proxy1?limit=100 > /dev/null;
sleep 5;

#start java code
echo "Starting the java code"
cd ../CapacityEstimator;
bash run.sh;

#start lighttpd
echo "Starting the lighttpd server";
ssh root@$tokencheck "bash /home/webq/tokencheck/run.sh";

echo "Setting $server governor to performance:"
for i in `seq 0 3`; do
	ssh root@$server "cpufreq-set -g performance -c $i";
done;
echo "Checking if governor is set correctly:"
ssh root@$server "cpufreq-info | grep \"governor \"";

echo "################# REDEPLOYMENT ATTEMPT FINISHED ##################";

