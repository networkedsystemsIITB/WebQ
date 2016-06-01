 #!/bin/bash
 for i in $(seq 20 20 160);
 do
    echo $i"**************"
    httperf  \
        --hog                       \
        --timeout=15                 \
        --server=10.129.49.76       \
        --rate=$i                   \
        --add-header="Content-Type: application/x-www-form-urlencoded\n" \
        --wsesslog=1200,1,httperf_content | grep Reply
        # --print-reply                                                    \
 done
