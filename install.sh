#!/usr/bin/env bash

# Put database to the home directory by default
db_path=$HOME/.hm

# Write scripts to .bashrc by default
target=$HOME/.bashrc

while [[ $# > 0 ]]
do
  key=$1
  
  case $key in
    --db)
    db_path=$2
    ;;
    --target)
    target=$2
    ;;
    *)
    echo "Unknown option $1 $2"
    ;;
  esac  
  
  shift
  shift
done

echo "
# Create History Manager folder where database will be placed
mkdir -p $db_path

# Create a variable with path to the database
hm_history_db=$db_path/history.db

# Create a variable with path to the temporary history file
hm_history_tmp=$db_path/history.tmp

" >> $target

cat ./scripts/scripts-to-bashrc >> $target
