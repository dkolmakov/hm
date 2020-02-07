
get_last_command() {
    if [ $BASH_VERSION ]
    then
        echo $(history 3 | head -n 1 | sed -E "s/^ *[0-9]* *//")
    fi
    
    if [ $ZSH_VERSION ]
    then
        echo $(echo "$(fc -l -2)" | head -n 1 | sed -E "s/^ *[0-9]* *//")
    fi
}

rc=0

# Check that the test command is in history (second from the end, since the first is "hm -d /test/path")
last_command=$(get_last_command)
if ! [ "$last_command" = "$test_command" ]; then echo 'Error!!! Test command not found!'; rc=1; fi

# Form error code 
[ $rc -eq 0 ]
