#!/usr/bin/env bash

rc=0

# Check database file
if [ -z $hm_history_db ] || ! [ -f $hm_history_db ]; then echo "Error: no database found!"; rc=1; fi

# Check current session id
if [ -z $hm_session_id ] || ! [ $hm_session_id -ge 1 ]; then echo "Error: wrong session id!"; rc=1; fi
    
# Add test entry to the database
hm-db --db $hm_history_db -a $hm_session_id "$(date +"%Y-%m-%d %H:%M:%S")" /test/path "test command" 0

# Select commands from the test path
hm -d /test/path 

# Check that the test command is in history (second from the end, since the first is "hm -d /test/path")
if ! [ "$(history 2 | head -n 1 | sed -E "s/^ *[0-9]* *//")" == "test command" ]; then echo "Error: test command is not found!"; rc=1; fi

# Form error code 
[ $rc -eq 0 ]
