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

enum Type {
    ADD_SESSION = 0,
    ADD_CMD
};

enum ErrorCode {
    SUCCESS = 0,
    ARG_ERROR,
    FAILED_TO_ADD
};

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " --db PATH -t NUMBER [--sess_name STRING] [--sess_id NUMBER --pwd PATH --cmd STRING] "
              << "Options:\n"
              << "\t-h,--help\ttShow this help message\n"
              << "\t-db PATH\tPath to the history database\n"
              << "\t-t NUMBER\tType of an entry to add:\n\t\t\t    0 - new session, unique identifier is returned\n\t\t\t    1 - new command\n"
              << "\tsess_name STRING\tName of a session to be created\n"
              << "\tsess_id STRING\tUnique session identifier\n"
              << "\tpwd PATH\tPath to the directory where command is executed\n"
              << "\tcmd STRING\tExecuted command\n"
              << std::endl;
}


int main(int argc, char* argv[]) {

    if (argc < 7) {
        show_usage(argv[0]);
        return ARG_ERROR;
    }

    std::string db_path = "/not/defined.db";
    int type = 0;

    std::string sess_name = "not defined";

    int sess_id = 0;
    std::string pwd = "not defined";
    std::string cmd = "not defined";

    for (int i = 1; i < argc; i+=2) {
        std::string arg = argv[i];

        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return SUCCESS;
        } else {
            if (i + 1 < argc) {
                if (arg == "--db") {
                    db_path = argv[i + 1];
                }
                else if (arg == "-t") {
                    type = atoi(argv[i + 1]);
                }
                else if (arg == "--sess_name") {
                    sess_name = argv[i + 1];
                }
                else if (arg == "--sess_id") {
                    sess_id = atoi(argv[i + 1]);
                }
                else if (arg == "--pwd") {
                    pwd = argv[i + 1];
                }
                else if (arg == "--cmd") {
                    cmd = argv[i + 1];
                }
                else {
                    std::cout << "ERROR! Unrecognized option: " << argv[i] << " " << argv[i + 1] << std::endl;
                    show_usage(argv[0]);
                    return ARG_ERROR;
                }
            } else {
                std::cout << "ERROR! No option value!" << std::endl;
                show_usage(argv[0]);
                return ARG_ERROR;
            }
        }
    }


    try {
        History history(db_path);

        switch (type) {
        case ADD_SESSION:
            std::cout << history.insert_sess(sess_name);
            break;
        case ADD_CMD:
            history.insert_cmd(sess_id, pwd, cmd);
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
