### Required Packages

- libssl-dev 
- libbz2-dev

### Configuration steps
The configuration options are in the file lighttpd.conf. 
- Configure CapacityEstimator ip and post at btp.h in lighttpd/src folder.
( this should be same as `webq.tokencheck.port` in
`CapacityEstimator/webq.properties` ). Do the install steps if the
CapacityEstimator ip is changed.
- The IP of test moodle server ip is configured in TokenCheck/lighttpd/lighttpd.conf.

### Install steps

1. Go inside "lighttpd" directory.
2. Run `sudo make`, and then `sudo make install`.
3. For starting the server, run `lighttpd -f lighttpd.conf`.This is
   done as a part of `redeploy.sh` and is not expected to run
   manually.

###  Code changes to lighttpd
Code changes on lighttpd for this project are in TokenCheck/lighttpd/src/btp.h.
The lighttpd error log can be found at /var/log/lighttpd/error.log.
