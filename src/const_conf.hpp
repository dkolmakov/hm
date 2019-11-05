/*
   Copyright 2019 Dmitry Kolmakov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

#include <string>

const std::string const_configuration = "\
# Create history manager home directory\n\
mkdir -p $hm_home\n\
\n\
# Path to the HM database\n\
hm_history_db=$hm_home/history.db\n\
\n\
# Path to the temporary history file\n\
hm_history_tmp=$hm_home/history.tmp\n\
\n\
# Create a new session entry in the database corresponding to the current terminal session\n\
hm_session_id=$(hm-db $hm_history_db session)\n\
\n\
# Service string to split values of stored commands\n\
separator=\"!!hmseparator!!\"\n\
\n\
# Set a script to be executed after each other command in the current terminal session\n\
export PROMPT_COMMAND='rc=$?; if [ $(id -u) -ne 0 ] && ! [ -z $hm_session_id ]; then echo ${hm_session_id}${separator}$(date +\"%Y-%m-%d %H:%M:%S\")${separator}$(pwd)${separator}\"$(history 1 | sed -E \"s/^ *[0-9]* *//\")\"${separator}$rc >> $hm_history_tmp; fi'\n\
\n\
# Wrapper for hm-db which adds the selected commands at the beginning of the current terminal history\n\
hm() {\n\
SAVE_HISTFILE=$HISTFILE\n\
HISTFILE=$(mktemp extracted_history.XXXXXXXXXX)\n\
# Parse commands saved to the text file\n\
hm-db $hm_history_db parse $hm_history_tmp $separator\n\
# Update history\n\
hm-db $hm_history_db select $@ > $HISTFILE && echo hm \"$*\" >> $HISTFILE && history -r\n\
if [ $? -eq 0 ]; then echo \"History successfully updated!\"; fi\n\
rm -rf $hm_history_tmp $HISTFILE\n\
HISTFILE=$SAVE_HISTFILE\n\
}\n\
";
