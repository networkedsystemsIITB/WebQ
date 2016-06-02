#WebQ

WebQ is a system consisting of two web proxies, that together simulate a virtual
queue of web requests, and shape incoming load to match server capacity.

# Test Setup Installation

redeploy.sh script at `bin/` is used to bring up all the components in differant machines

Before redeploy can be succesfully run some configuraions have to done on respective machines
running the components.

for ease of refferal we will reffer to machines as such

component running  | mahcine name
------------------ | ---------------
TokenGen           | MTG
TokenCheck         | MTC
CapacityEstimator  | MCE
TestServer         | MTS
redeploy.sh        | PRIMARY

1. clone this repo in each of the machines. One machine may run multiple componets and can also double up as a PRIMARY machine as well.
    for eg:
        TokenGen and CapacityEstimator and run at one machine and that machine can double up as PRIMARY machine

2. At PRIMARY machine grant passwordless root access to all other machines
3. At MTG update proxy.conf with correct TokenCheck ip
4. At MTC update lighttpd.conf with correct server ip. follow instruction in doc/README and make lighttpd
5. At MTS install test scripts which will be access by user.
6. run redeploy.sh from PRIMARY machine
