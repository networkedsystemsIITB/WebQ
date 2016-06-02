#WebQ

WebQ is a system consisting of two web proxies, that together simulate a virtual
queue of web requests, and shape incoming load to match server capacity.

# Installation

To get WebQ up and running , TokenGen , TokenCheck and CapacityEstimator have to
be successfull installed and configured. Ideally TokenGen and CapacityEstimator
are run on one machine( or VM ) and TokenCheck on another. The instllation
instructions for each of the components are found in respective `doc/` folders
TokenGen should be configured with IP of TokenCheck
TokenCheck should be configured with IP of server backend whose traffic WebQ is shaping
CapacityEstimator should be configured with ip of TokenGen as well as TokenCheck
How and where to configure these are detailed in the respective `doc/` folders


To start components, just run `redeploy.sh` in `bin/` folder. Do not deploy each
component by hand, as there are many dependencies between the modules, and there
is a sequence to start things, which is all taken care in the `redeploy.sh`
script.
