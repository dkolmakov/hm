
rc=0

# Check that the test command is in history (second from the end, since the first is "hm -d /test/path")
last_command=$(get_last_command)
if ! [ "$last_command" = "echo test" ]; then echo "Error! $last_command != echo test"; rc=1; fi

# Form error code 
[ $rc -eq 0 ]
