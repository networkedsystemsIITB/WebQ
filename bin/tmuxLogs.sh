#!/bin/bash
if [ -z "$TMUX" ]; # if not inside tmux
then
    echo outside tmux
    tmux start-server
    tmux new-session -d
    # tmux new-window
fi
# tmux select-window -tHaystack:1
tmux split-window
tmux split-window
tmux select-pane -t :.0
tmux split-window -h
tmux select-pane -t :.2
tmux split-window -h
# select pane 0 of current window


#commands
C1='tail -f /etc/apache2/error.log'
C2='tail -f /usr/lib/cgi-bin/proxy1.log'
C3='tail -f ~/webq/CapacityEstimator/javapersecond.log'
C4='tail -f ~/webq/CapacityEstimator/javadebug.log'

# INFO 
# set pane title using 
# ESC ]2; ... ESC \
# printf '\033]2;%s\033\\' 'title goes here'
tmux send-keys -t:.0 "printf '\033]2;%s\033\\' $C1" C-m
tmux send-keys -t:.1 "printf '\033]2;%s\033\\' $C2" C-m
tmux send-keys -t:.2 "printf '\033]2;%s\033\\' $C3" C-m
tmux send-keys -t:.3 "printf '\033]2;%s\033\\' $C4" C-m


tmux send-keys -t:.0 "$C1" C-m
tmux send-keys -t:.1 "$C2" C-m
tmux send-keys -t:.2 "$C3" C-m
tmux send-keys -t:.3 "$C4" C-m

if [ -z "$TMUX" ]; # if not inside tmux
then
tmux attach-session
fi
