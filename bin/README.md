### Scripts for running tests

The scripts are listed in the order of importance


1. `redeploy.sh` - The most frequently used script for deploying the
   code.  The script internally access `killall.sh` and `ips.sh`.  The
   host machine should have `sshpass` and `lynx` installed for this
   script to run successfully.  The script requires ssh access to all
   the component machines.  Use `makeKnownHost.sh` to configure
   passwordless ssh access to machines.  `redeploy.sh` requires access
   to root account of the participating machines. This has to be
   setup.  In Ubuntu you have to explicitly enable the root login.
   [Enable root login in Ubuntu](http://askubuntu.com/questions/44418/how-to-enable-root-login)
2. `ips.sh` - This file serves as as a single point for modifying ips
   of the current deployment. The variables in this script must be
   updated to correct values for `redeploy.sh` to function.
3. `startupTmux.sh` - a helper script to open up windows in tmux
   monitoring all relevant log files in different machines . This
   script is strictly for development ease and is not necessary for
   running WebQ.
4. `dev_setup.sh` - execute this file after the first time you clone the repo.
5. `cscope.sh` - a simple script to generate the file list needed for cscope
6. `makeKnownHost.sh` - Setup a password less login to target machine
   and user. (same job as `ssh-copy-id`)
