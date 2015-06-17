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

tmux send-keys -tHaystack:1.0 "ssh murali@gen" C-m
tmux send-keys -tHaystack:1.1 "ssh murali@check" C-m
tmux send-keys -tHaystack:1.2 "ssh murali@server" C-m
tmux send-keys -tHaystack:1.3 "ssh murali@vacha" C-m

tmux attach-session -d -tHaystack
