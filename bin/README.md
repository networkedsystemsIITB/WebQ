### Scripts for running tests

The scripts are listed in the order of importance


1. `redeploy.sh` - The most frequnquently used script for deploying the code.
The script internally access `killall.sh` and `ips.sh`.
The host machine should have `sshpass` and `lynx` installed for this script to run successfully
2. `ips.sh` - This file serves as as a single point for modifying ips of the 
current deployment. The variables in this script must be updated to correct values 
for `redeploy.sh` to function

3. `startupTmux.sh`
4. `dev_setup.sh` - execute this file after the first time you clone the repo!
5. `cscope.sh`
6. `makeKnownHost.sh` - a script that does the same job as `ssh-copy-id`
