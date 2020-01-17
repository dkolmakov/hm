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
\t# Create history manager home directory\n\
\tmkdir -p $hm_home\n\
\n\
\t# Path to the HM database\n\
\thm_history_db=$hm_home/history.db\n\
\n\
\t# Path to the temporary history file\n\
\thm_history_tmp=$hm_home/history.tmp\n\
\n\
\t# Create a new session entry in the database corresponding to the current terminal session\n\
\thm_session_id=$(hm-db $hm_history_db session)\n\
\n\
\t# Service string to split values of stored commands\n\
\tseparator=\"!!hmseparator!!\"\n\
\n\
\t# Set a script to be executed after each other command in the current terminal session\n\
\texport PROMPT_COMMAND='rc=$?; if [ $(id -u) -ne 0 ] && ! [ -z $hm_session_id ]; then echo ${hm_session_id}${separator}$(date +\"%Y-%m-%d %H:%M:%S\")${separator}$(pwd)${separator}\"$(history 1 | sed -E \"s/^ *[0-9]* *//\")\"${separator}$rc >> $hm_history_tmp; fi'';'$PROMPT_COMMAND\n\
\n\
\t# Wrapper for hm-db which adds the selected commands at the beginning of the current terminal history\n\
\thm() {\n\
\t\t# Parse arguments\n\
\t\tARGS=""\n\
\t\tfor i in \"$@\"\n\
\t\tdo \n\
\t\tcase $i in \n\
\t\t\t-v|--version)\n\
\t\t\thm-db --version\n\
\t\t\t;;\n\
\t\t\t-h|--help)\n\
\t\t\thm-db --help\n\
\t\t\t;;\n\
\t\t\t-i|--info)\n\
\t\t\thm-db $hm_history_db info $hm_session_id\n\
\t\t\t;;\n\
\t\t\t*)\n\
\t\t\tARGS=\"$ARGS $i\"\n\
\t\t\t;;\n\
\t\tesac\n\
\t\tdone\n\
\n\
\t\t# Perform history recovery\n\
\t\tif ! [ -z \"$ARGS\" ]\n\
\t\tthen\n\
\t\t\tSAVE_HISTFILE=$HISTFILE\n\
\t\t\tHISTFILE=$(mktemp extracted_history.XXXXXXXXXX)\n\
\t\t\t# Parse commands saved to the text file\n\
\t\t\thm-db $hm_history_db parse $hm_history_tmp $separator\n\
\t\t\t# Update history\n\
\t\t\thm-db $hm_history_db select $hm_session_id $ARGS > $HISTFILE && echo hm \"$*\" >> $HISTFILE && history -r\n\
\t\t\tif [ $? -eq 0 ]; then echo \"History successfully updated!\"; fi\n\
\t\t\trm -rf $hm_history_tmp $HISTFILE\n\
\t\t\tHISTFILE=$SAVE_HISTFILE\n\
\t\tfi\n\
\t}\n\
";
