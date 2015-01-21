#!/bin/sh

tokencheck="wolverine2"

#tokencheckscript="run.sh"
tokencheckscript="run_twoservers.sh"

#kill the current server
ssh root@$tokencheck "killall lighttpd &> /dev/null";

#start lighttpd
echo "Starting the lighttpd server";
ssh root@$tokencheck "bash /home/webq/tokencheck/"$tokencheckscript;

echo "################# CAPACITY INCREASED SUCCESSFULLY ##################";

