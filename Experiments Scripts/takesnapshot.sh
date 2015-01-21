#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <snapshot_folder_name>";
	exit 1;
fi

dest=/home/webq/Experiments/$1
mkdir $dest

cp /home/webq/CapacityEstimator/javapersecond.log $dest/
cp /home/webq/CapacityEstimator/javadebug.log $dest/
cp /home/webq/CapacityEstimator/webq.properties $dest/

cp /home/webq/CapacityEstimator/overloadpoints.java.log $dest/
cp /home/webq/CapacityEstimator/goodpoints.java.log $dest/

cp /home/webq/TokenGenNew/proxy1.log $dest/
cp /home/webq/TokenGenNew/proxy1.c $dest/
scp vachaspati:/home/webq/summary60.csv $dest/jmetersummary60.csv
scp vachaspati:/home/webq/summary110.csv $dest/jmetersummary110.csv
#scp bhavin@pollaczek:/home/bhavin/summary.csv $dest/jmetersummary.csv
date > $dest/time_of_snapshot

echo "Done taking snapshot";
echo "In folder: $dest";

~/scripts2/plot.sh $1
