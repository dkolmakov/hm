#/usr/bin/env bash

hm-select ~/.hm/history.db "$(pwd)" > ~/.bash_history 
history -r

