### TokenCheck install steps

install `libssl-dev` and `libbz2-dev` package 

1. Go inside "lighttpd" directory.
2. Run `sudo make`, and then `sudo make install`


For starting the server, run `lighttpd -f lighttpd.conf`

### TokenCheck configuration steps

The configuration options are in the file lighttpd.conf. CapacityEstimator ip and port at which it
listens ( this is listed as webq.tokencheck.port in `CapacityEstimator/webq.properties` ) is HARD
CODED in btp.h !!!!
The Apache server ip is configured in TokenCheck/lighttpd/lighttpd.conf  

###  Code changes to lighttpd
Code changes for this project to lighttpd are in TokenCheck/lighttpd/src/btp.h

### Other files
the lighttpd error log can be found at /var/log/lighttpd/error.log
