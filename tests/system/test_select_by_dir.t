Deploy history manager, create test variables and put special command to the history

  $ source "$TESTDIR"/prepare_test_env.sh

Create test directory

  $ mkdir -p $test_path

Go to test path

  $ cd $test_path

Perform test command

  $ echo test
  test

Put several dummy commands on top of history in different directory 

  $ cd $TESTDIR
  $ echo wrong
  wrong

Recover history for the current directory

  $ hm -d $test_path > /dev/null

Check last command in the history

  $ source "$TESTDIR"/check_test_result.sh
