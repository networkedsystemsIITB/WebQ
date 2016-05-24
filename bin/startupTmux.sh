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

tmux send-keys -tHaystack:1.0 "ssh gen2" C-m
tmux send-keys -tHaystack:1.1 "ssh check" C-m
tmux send-keys -tHaystack:1.2 "ssh gen1" C-m
tmux send-keys -tHaystack:1.3 "ssh vacha" C-m
# till now 4 windows.
# add 4 more:
if [ -z "$1" ];
then
    # do nothing if no parameter is specified
    :
else
    tmux select-pane -t Haystack:1.0
    tmux split-window -h
    tmux select-pane -t Haystack:1.2
    tmux split-window -h
    tmux select-pane -t Haystack:1.4
    tmux split-window -h
    tmux select-pane -t Haystack:1.6
    tmux split-window -h
    tmux send-keys -tHaystack:1.1 "ssh server" C-m
    tmux send-keys -tHaystack:1.3 "ssh spare1" C-m
    tmux send-keys -tHaystack:1.5 "ssh spare2" C-m
    tmux send-keys -tHaystack:1.7 "ssh murali@10.14.15.22" C-m
fi
tmux attach-session -d -tHaystack
