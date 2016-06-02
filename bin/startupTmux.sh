#!/bin/sh
tmux start-server
tmux new-session -d -s Haystack -n git
tmux new-window -tHaystack:1 -n all
tmux select-window -tHaystack:1
tmux split-window
tmux select-pane -t Haystack:1.0
tmux split-window -h
tmux select-pane -t Haystack:1.2
tmux split-window -h
tmux select-pane -t Haystack:1.0
tmux split-window -h
tmux select-pane -t Haystack:1.2
tmux split-window -h
tmux select-pane -t Haystack:1.4
tmux split-window -h
tmux select-pane -t Haystack:1.6
tmux split-window -h
tmux send-keys -tHaystack:1.0 "ssh gen2" C-m
tmux send-keys -tHaystack:1.1 "ssh spare1" C-m
tmux send-keys -tHaystack:1.2 "ssh gen1" C-m
tmux send-keys -tHaystack:1.3 "ssh check" C-m
tmux send-keys -tHaystack:1.4 "ssh server" C-m
tmux send-keys -tHaystack:1.5 "ssh vacha" C-m
tmux send-keys -tHaystack:1.6 "ssh spare2" C-m
tmux send-keys -tHaystack:1.7 "" C-m #left free

# now the tokengen logs window ; expand as requried 
tmux new-window -tHaystack:2 -n logs
tmux select-window -tHaystack:2
tmux split-window
tmux select-pane -t Haystack:2.0
tmux split-window -h
# logs at gen1 gen2
tmux send-keys -tHaystack:2.0 "ssh gen2" C-m
tmux send-keys -tHaystack:2.0 "watch -n1 -t 'echo gen2 & tail /usr/lib/cgi-bin/proxy1.log'" C-m
tmux send-keys -tHaystack:2.1 "ssh gen1" C-m
tmux send-keys -tHaystack:2.1 "watch -n1 -t 'echo gen1 & tail /usr/lib/cgi-bin/proxy1.log'" C-m
tmux send-keys -tHaystack:2.2 "ssh spare1" C-m
tmux send-keys -tHaystack:2.2 "watch -n1 -t 'echo spare1 & tail /usr/lib/cgi-bin/proxy1.log'" C-m

# now capacity estimator window
tmux new-window -tHaystack:3 -n caplogs
tmux select-window -tHaystack:3
tmux split-window
tmux send-keys -tHaystack:3.0 "tail -f ~/WebQ/CapacityEstimator/javadebug.log" C-m
tmux send-keys -tHaystack:3.1 "tail -f ~/WebQ/CapacityEstimator/javapersecond.log" C-m

tmux new-window -tHaystack:4 -n workarea
tmux attach-session -d -tHaystack
