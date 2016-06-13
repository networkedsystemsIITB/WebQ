### Installing TokenGen

**Machine Setup**
Configure FastCGI for apache in the target machine. Sample `apache2_etc.conf` file is provided in
the repo for reference.

**Configuration**
The configuration setting for each TokenGen is stored in proxy.conf.
Each of the lines store a respective setting:

1. port at where proxy1 should listen.
2. token check ip.
3. number of peer proxies.

subsequent lines have the ips of the peers ( there should be as many lines
below as the number of peers mentioned above

sample contents of `proxy.conf`

    10000                           //port at which proxy 1 should listen
    10.129.41.67:9000               // token check ip
    2                               // number of peers (excluding current)
    10.129.26.133                   //ip of peers
    10.129.28.32                    //ip of peers

**Installation**
To make and install TokenGen manually run `./make_script.sh`.
Usually this is handled automatically by the `bin/redeploy.sh`

**Logs**
The logs of TokenGen can be found at ` /usr/lib/cgi-bin/proxy1.log `
