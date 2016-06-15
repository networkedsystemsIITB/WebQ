### Seting Up Moodle server

#### Required Packages

- php5-curl
- php5-gd

#### Installation Instructions

Use [Moodle Performance Comparison](https://github.com/moodlehq/moodle-performance-comparison).
to setup test moodle installations.

At /var/www/html clone moodle-performance-analysis as moodleM, moodleXS, moodleS

**moodle-performance-analysis configuration:** the following fields of
`webserver_config.properties` at
`moodle(XS|S|M)/webserver_config.properties`  were altered to meet the
local machine requirements (these are sample values, use the
dbpassword of your machine the below fields)

```
dbtype="mysqli"
dbuser="root"
dbpass="webqwebq"
wwwroot="http://10.129.49.76/moodleM/moodle"
dataroot="/var/www/moodledataM/"
repository="https://github.com/moodle/moodle.git"
beforebranchrepository="https://github.com/moodle/moodle.git"
afterbranchrepository="https://github.com/moodle/moodle.git"
backupsdir="/tmp"
```
moodle-performance-comparison provide various options to generate the dummy
data of students and courses (XS -Extra Small , S- Small etc )
```
cd /path/to/moodle-performance-comparison (in this case /path/to/moodleM) 
./before_run_setup.sh {XS|S|M|L|XL|XXL} 
```
A moodle populated with various number of courses can be created based on the 
argument passed ( XS , S etc )

When server is accessed from TokenCheck (through lighttpd proxy) the ip
used will be different and hence will trigger is redirect. Use the following 
setting to prevent such redirects:

In each of `moodle(XS|S|M)/moodle/config.php:19`
```
$CFG->wwwroot   = 'http://'.$_SERVER["HTTP_HOST"].'/moodleXS/moodle'; 
```



for e.g: if `moodleM` is medium-Moodle installation setup at `/var/www/html/`,<br/>
    put script req1.php and req2.php in /var/www/html/moodleM/moodle/webq<br/>
    and use:
```
http://<tokegenip>:<port>/proxy1?limit=/moodleM/moodle/webq/req1.php?a=b
```
to access via WebQ when server ip is properly configured in TokenCheck.
