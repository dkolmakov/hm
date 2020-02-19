# Prepare common test environment

get_last_command() {
    if [ $ZSH_VERSION ]
    then
        echo $(echo "$(fc -l -2)" | head -n 1 | sed -E "s/^ *[0-9]* *//")
    else
        echo $(history 2 | head -n 1 | sed -E "s/^ *[0-9]* *//")
    fi
}

rc=0

# Check database file
if [ -z $hm_history_db ] || ! [ -f $hm_history_db ]; then echo "Error: no database found!"; rc=1; fi

# Check current session id
if [ -z $hm_session_id ] || ! [ $hm_session_id -ge 1 ]; then echo "Error: wrong session id!"; rc=1; fi
    
# Create unique test command and test path
# test_root="$(pwd)"
# test_path_outer="$(pwd)/test/path"
test_path="$(pwd)/test/path/$(date +"%Y-%m-%d_%H:%M:%S")"
test_session_name="$(date +"%Y-%m-%d_%H:%M:%S")"
# test_command="touch $(date +"%Y-%m-%d %H:%M:%S")"

# Create test session name in the database
# test_session_id=$(hm-db $hm_history_db session $test_session_name)
# if ! [ $? -eq 0 ]; then echo "Error!!! Failed to create session!"; fi
# 
# Add test entry to the database
# hm-db $hm_history_db add $hm_session_id "$(date +"%Y-%m-%d %H:%M:%S")" $test_path "$test_command" 0
# if ! [ $? -eq 0 ]; then echo "Error!!! Adding entry to database failed!"; fi
# 
# Select commands from the test path
# hm -d $test_path
# if ! [ $? -eq 0 ]; then echo "Error!!! Selecting commands failed!"; rc=1; fi
# 

# Form error code 
[ $rc -eq 0 ]
