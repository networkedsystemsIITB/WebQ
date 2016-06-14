## WebQ

Servers often crash when faced with transient overload, causing user
experience to degrade sharply when incoming load exceeds capacity.
Existing overload control mechanisms focus on some form of admission
control to protect the web server from overload. Importantly, there is
no feedback to the frustrated user about when to retry again, leading
to adhoc retries.

WebQ consist of two web proxies, that together simulate a virtual
queue of web requests, and shape incoming load to match server
capacity. Users accessing a server protected by WebQ receive a HTTP
redirect response specifying a wait time in the virtual queue, and are
automatically redirected to the web server upon expiration of the wait
time. The wait times are calculated using an estimate of the server’s
capacity that is computed by WebQ.  Users in the virtual queue are
provided with a secure cryptographic token, which is checked to
guarantee that the user has waited his prescribed time in the queue.

### Directory Structure

- **bin** - Scripts to deploy all the components.
- **TokenCheck** - Source code and documentation of Token Check.
- **TokenGen** - Source code and documentation of Token Gen.
- **ServerScripts** - Scripts and details on how to setup a test Moodle server.
- **CapacityEstimator** - Source code and documentation of Capacity Estimator.
- **JMeter_test_configurations** - Test configuration used when testing with jmeter [deprecated]. Use [LoadGenerator](https://github.com/networkedsystemsIITB/LoadGenerator) instead.

#### Developer Manual

Please find the document *developer_manual.pdf* under the *doc* folder
of respective folders. It contains detailed instructions for
understanding the codebase. It will be helpful while modifying/adding
code functionalities to WebQ.


### Configuration and Machine Setup

`redeploy.sh` script at `bin/` is used to bring up all the components
in different machines.  Before `redeploy.sh` can be successfully run
some configurations have to done on respective machines running the
components.

1. Clone this repo in each of the machines. One machine may run
   multiple components. for e.g: TokenGen and CapacityEstimator and
   run at one machine and that machine can run redeploy.sh as well.
2. Chose one machine as a PRIMARY machine and grant password-less root
   access to all other machines. This machine is will run
   `redeploy.sh`.
3. At TokenGen machines update `proxy.conf` with correct TokenCheck ip.
4. At TokenCheck configure `lighttpd.conf` with correct proxy server ip.
    Configure CapacityEstimator ip in `btp.h`.
    Make and build TokenCheck.(Detailed configuration and build instructions available in TokenCheck/README.md).
5. At CapacityEstimator install Matlab and java. Configure `webq.properties` file with correct TokenGen ip and port.
6. At TestServer setup [Test Moodle](https://github.com/moodlehq/moodle-performance-comparison).
   Install test scripts available at `ServerScripts` (See
   ServerScripts README).
7. Run `redeploy.sh` from PRIMARY machine to start using WebQ for
   TestServer.


### Publications

* [WebQ: A Virtual Queue To Improve User Experience During Web Server Overload](https://www.cse.iitb.ac.in/~mythili/research/webq-iwqos.pdf)</br>
Bhavin Doshi, Chandan Kumar, Pulkit Piyush, Mythili Vutukuru</br>
IEEE/ACM International Symposium on Quality of Service (IWQoS), Portland, OR, June 2015.



