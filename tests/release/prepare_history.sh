
rc=0

# Check database file
if [ -z $hm_history_db ] || ! [ -f $hm_history_db ]; then echo "Error: no database found!"; rc=1; fi

# Check current session id
if [ -z $hm_session_id ] || ! [ $hm_session_id -ge 1 ]; then echo "Error: wrong session id!"; rc=1; fi
    
# Create unique test command and test path
test_command="test command $(date +"%Y-%m-%d %H:%M:%S")"
test_path="/test/path/$(date +"%Y-%m-%d_%H:%M:%S")"
    
# Add test entry to the database
hm-db $hm_history_db add $hm_session_id "$(date +"%Y-%m-%d %H:%M:%S")" $test_path "$test_command" 0
if ! [ $? -eq 0 ]; then echo "Error!!! Adding entry to database failed!"; rc=1; fi

# Select commands from the test path
hm -d $test_path
if ! [ $? -eq 0 ]; then echo "Error!!! Selecting commands failed!"; rc=1; fi

# Form error code 
[ $rc -eq 0 ]
