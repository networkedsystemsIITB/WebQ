#WebQ

WebQ is a system consisting of two web proxies, that together
simulate a virtual queue of web requests, and shape incoming load
to match server capacity.

# Installation

read README of TokenGen 

read README of TokenCheck 

read README ofCapacityEstimator

To start components, just run redeploy.sh in "Experiments
Scripts" folder. Do not deploy each component by hand, as there
are many dependencies between the modules, and there is
a sequence to start things, which is all taken care in the
redeploy.sh script.
