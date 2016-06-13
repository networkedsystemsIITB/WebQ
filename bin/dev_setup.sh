#!/bin/bash

#run this script to fix some repetitve tasks when first setting up 
# the development machine

# some binary files have been inadventantly commited into git tree previously.
# use this script to prevent them from bothering you
cd ~/WebQ
git update-index --assume-unchanged CapacityEstimator/*.log
git update-index --assume-unchanged TokenCheck/lighttpd/config.h
git update-index --assume-unchanged TokenCheck/lighttpd/config.log
git update-index --assume-unchanged TokenCheck/lighttpd/config.status
git update-index --assume-unchanged TokenCheck/lighttpd/libtool
git update-index --assume-unchanged TokenCheck/lighttpd/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/**/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/src/.deps/mod_compress.Plo
git update-index --assume-unchanged TokenCheck/lighttpd/src/.libs/**
git update-index --assume-unchanged TokenCheck/lighttpd/src/lighttpd
git update-index --assume-unchanged TokenCheck/lighttpd/src/mod_compress.la
git update-index --assume-unchanged TokenCheck/lighttpd/src/*.o
git update-index --assume-unchanged TokenCheck/lighttpd/src/proc_open
git update-index --assume-unchanged TokenCheck/lighttpd/src/spawn-fcgi
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/123/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/www/expire/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/www/go/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/www/indexfile/Makefile
git update-index --assume-unchanged TokenCheck/lighttpd/tests/docroot/www/Makefile
git update-index --assume-unchanged TokenGen/src/proxy1
git update-index --assume-unchanged TokenGen/src/proxy1.log
git update-index --assume-unchanged TokenGen/src/waittimedebug.log
