
test_dir=$1
cd $test_dir

rc=0

for test in *.test
do
  if [ -f $test ] 
  then
    source $test
    echo "$test: $(if [ $? -eq 0 ]; then echo "passed"; else echo "failed"; fi)"
  fi
done

# Form error code 
[ $rc -eq 0 ]
