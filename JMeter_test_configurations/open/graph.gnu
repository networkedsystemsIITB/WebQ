set terminal pngcairo

set title 'Goodput vs Request/sec'
set ylabel 'Request completed per sec'
set xlabel 'Reqests per sec'
set output 'throughput.png'
plot 'throughput.dat' with lp

set title 'Response time vs Request/sec'
set ylabel 'Response time in milliseconds'
set xlabel 'Reqests per sec'
set output 'responsetime.png'
plot 'responsetime.dat' with lp
