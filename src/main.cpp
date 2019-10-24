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

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <iostream>

#include "history.hpp"
#include "version.hpp"

enum Type {
    ADD_SESSION = 1,
    ADD_CMD,
    SELECT_BY_PATH,
};

enum ErrorCode {
    SUCCESS = 0,
    ARG_ERROR,
    FAILED_TO_ADD
};

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " --db PATH [-s SESSION_NAME] [-a SESSION_ID PATH CMD RET_CODE] [-d PATH [-R]]"
              << "Options:\n"
              << "\t-h,--help\ttShow this help message\n"
              << "\t-db PATH\tPath to the history database\n"
              << "\t-s SESSION_NAME\tCreates a new session and return its unique identifier.\n\t\t\t    SESSION_NAME - name of a session to be created.\n"
              << "\t-a SESSION_ID PATH CMD RET_CODE\tAdds a new entry to the command history database.\n\t\t\t    SESSION_ID - session identifier,\n\t\t\t    PATH - working directory,\n\t\t\t    CMD - executed command,\n\t\t\t    RET_CODE - result code.\n"
              << "\t-d PATH\tPerforms a selection by the working directory column.\n"
              << "\t-R\tWorks torgether with -d option. Changes selection to be recursive and accept all commands executed in the specified directory and all directories down by hierarchy.\n"
              << std::endl;
}

static void show_version(std::string name)
{
    std::cout << name << " " << HM_VERSION << std::endl
              << "Copyright (C) 2019 Dmitry Kolmakov." << std::endl
              << "License ASLv2.0: Apache License, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>." << std::endl
              << "This is free software: you are free to change and redistribute it." << std::endl
              << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
}

int main(int argc, char* argv[]) {

    std::string db_path = "/not/defined.db";
    int type = 0;
    bool parse_file = false;

    std::string sess_name = "not defined";

    int sess_id = 0;
    std::string datetime = "1972-01-01 00:00:00";
    std::string pwd = "not defined";
    std::string cmd = "not defined";
    std::string ret_code = "0";

    bool recursively = false;

    std::string filename = "/not/defined/filename";
    std::string separator = "notdefined";

    int i = 1;
    while (i < argc) {
        std::string arg = argv[i++];

        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return SUCCESS;
        } else {
            if (arg == "--db" && (i + 1) <= argc) {
                db_path = argv[i++];
            }
            else if (arg == "-a" && (i + 5) <= argc) {
                type = ADD_CMD;
                sess_id = atoi(argv[i++]);
                datetime = argv[i++];
                pwd = argv[i++];
                cmd = argv[i++];
                ret_code = argv[i++];
            }
            else if (arg == "-s" && (i + 1) <= argc) {
                type = ADD_SESSION;
                sess_name = argv[i++];
            }
            else if (arg == "-d" && (i + 1) <= argc) {
                type = SELECT_BY_PATH;
                pwd = argv[i++];
            }
            else if (arg == "-R") {
                recursively = true;
            }
            else if (arg == "-f" && (i + 1) <= argc) {
                parse_file = true;
                filename = argv[i++];
            }
            else if (arg == "-r" && (i + 1) <= argc) {
                separator = argv[i++];
            }
            else if (arg == "-v" || arg == "--version") {
                show_version(argv[0]);
                return 0;
            }
            else {
                std::cout << "ERROR! Wrong usage of option: \"" << arg << "\"" << std::endl;
                show_usage(argv[0]);
                return ARG_ERROR;
            }
        }
    }

    try {
        History history(db_path);
        
        if (parse_file)
            history.parse_input_file(filename, separator);

        switch (type) {
        case ADD_SESSION:
            std::cout << history.insert_sess(sess_name);
            break;
        case ADD_CMD:
            history.insert_cmd(sess_id, datetime, pwd, cmd, ret_code);
            break;
        case SELECT_BY_PATH:
            history.select_by_dir(pwd, recursively);
            break;
        default:
            std::cout << "ERROR! Unsupported type value!" << std::endl;
            show_usage(argv[0]);
            return ARG_ERROR;
        }

    }
    catch (int e)
    {
        return FAILED_TO_ADD;
    }

    return 0;
}

