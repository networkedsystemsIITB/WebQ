### Seting Up Moodle server

Use [Moodle Performance Comparison](https://github.com/moodlehq/moodle-performance-comparison).
to setup test moodle installations.

for e.g:
    if `moodleM` is medium-Moodle installation setup at `/var/www/html/`,
    put script req1.php and req2.php in /var/www/html/moodleM/moodle/webq
    and use:
    http://<tokegenip>:<port>/proxy1?limit=/moodleM/moodle/webq/req1.php?a=b
    to access via WebQ.
