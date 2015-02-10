 #!/bin/bash
 for i in $(seq 20 20 120);
 do
    respTime=`httperf  \
        --hog                       \
        --timeout=5                 \
        --server=10.129.49.76       \
        --rate=$i                   \
        --add-header="Content-Type: application/x-www-form-urlencoded\n" \
        --wsesslog=$(($i*60)),1,httperf_content | \
        grep Reply\ time | cut -d ' ' -f 5`
        # --print-reply                                                    \
    echo $i" "$respTime
 done
