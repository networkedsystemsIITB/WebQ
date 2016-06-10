## WebQ

WebQ is a system consisting of two web proxies, that together simulate
a virtual queue of web requests, and shape incoming load to match
server capacity.

### Directory Structure

- **TokenGenNew** - Source code and documentation of Token Gen.
- **TokenCheck** - Source code and documentation of Token Check.
- **CapacityEstimator** - Source code and documentation of Capacity Estimator.
- **bin** - Scripts to deploy all the components.
- **ServerScripts** - Scripts and details on how to setup a test Moodle server.
- **JMeter_test_configurations** - Test configuration used when testing with jmeter[deprecated]. Use [LoadGenerator](https://github.com/networkedsystemsIITB/LoadGenerator) instead.

#### Developer Manual

Please find the document *developer_manual.pdf* under the *doc* folder
of respective folders. It contains detailed instructions for
understanding the codebase. It will be helpful while modifying/adding
code functionalities to WebQ.

### Installation

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
4. At TokenCheck update `lighttpd.conf` with correct server ip. follow
   instruction in doc/README and make lighttpd.
5. At CapacityEstimator install Matlab and java.
6. At TestServer setup [Test Moodle](https://github.com/moodlehq/moodle-performance-comparison).
   Install test scripts available at `ServerScripts` (See
   ServerScripts readme).
7. Run `redeploy.sh` from PRIMARY machine.

### Publications

WebQ: A Virtual Queue To Improve User Experience During Web Server Overload [pdf](https://www.cse.iitb.ac.in/~mythili/research/webq-iwqos.pdf)</br>
Bhavin Doshi, Chandan Kumar, Pulkit Piyush, Mythili Vutukuru</br>
IEEE/ACM International Symposium on Quality of Service (IWQoS), Portland, OR, June 2015.
