#!/bin/bash

git clone https://github.com/muralisc/dotfiles
cd ~/dotfiles
./install.sh
cd ~/
git clone https://github.com/muralisc/webq
~/webq/bin/dev_setup.sh
