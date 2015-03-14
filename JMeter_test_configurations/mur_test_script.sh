

for threadno in {1..50}
do
    sed -i 's/threads">\([0-9]\+\)/threads">'$threadno'/' murali_tstMarch09.jmx
    sed -i 's/14-\([0-9]\+\)/14-'$threadno'/' murali_tstMarch09.jmx
done

