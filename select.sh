#/usr/bin/env bash -i

HISTFILE=$(mktemp extracted_history.XXXXXXXXXX)
hm --db ~/.hm/history.db -d \"$(pwd)\" -R > $HISTFILE 
history -r
rm -rf $HISTFILE
