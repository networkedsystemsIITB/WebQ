#!/bin/sh
tmux start-server
tmux new-session -d -s genlogs -n git
tmux new-window -tgenlogs:1 -n all
tmux select-window -tgenlogs:1
tmux split-window
tmux select-pane -t genlogs:1.0
tmux split-window -h
tmux select-pane -t genlogs:1.2
tmux split-window -h

tmux send-keys -tgenlogs:1.0 "ssh gen2" C-m
tmux send-keys -tgenlogs:1.0 "tmux" C-m
sleep 1
tmux send-keys -tgenlogs:1.0 "tail -f /usr/lib/cgi-bin/proxy1.log" C-m

tmux send-keys -tgenlogs:1.1 "ssh gen1" C-m
tmux send-keys -tgenlogs:1.1 "tmux" C-m
sleep 1
tmux send-keys -tgenlogs:1.1 "tail -f /usr/lib/cgi-bin/proxy1.log" C-m

tmux send-keys -tgenlogs:1.2 "ssh spare1" C-m
tmux send-keys -tgenlogs:1.2 "tmux" C-m
sleep 1
tmux send-keys -tgenlogs:1.2 "tail -f /usr/lib/cgi-bin/proxy1.log" C-m

tmux send-keys -tgenlogs:1.3 "ssh gen1" C-m
tmux send-keys -tgenlogs:1.3 "tail -f /etc/apache2/error.log" C-m
tmux attach-session -d -tgenlogs
