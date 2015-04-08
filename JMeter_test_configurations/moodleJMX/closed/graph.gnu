set terminal pngcairo

set title 'Goodput vs users'
set ylabel 'Request completed per sec'
set xlabel 'users (closed system)'
set output 'throughput.png'
plot 'throughput.dat' with lp

set title 'Response time vs users'
set ylabel 'Response time in milliseconds'
set xlabel 'users (closed system)'
set output 'responsetime.png'
plot 'responsetime.dat' with lp
