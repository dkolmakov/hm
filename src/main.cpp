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
#include "cxxopts.hpp"

enum ErrorCode {
    SUCCESS = 0,
    ARG_ERROR,
    FAILED_TO_ADD
};

static void show_version(std::string name)
{
    std::cout << name << " " << HM_VERSION << std::endl
              << "Copyright (C) 2019 Dmitry Kolmakov." << std::endl
              << "License ASLv2.0: Apache License, Version 2.0 <http://www.apache.org/licenses/LICENSE-2.0>." << std::endl
              << "This is free software: you are free to change and redistribute it." << std::endl
              << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
}

int main(int argc, char* argv[]) {

    try {
        cxxopts::Options options(argv[0], " - history manager for Bash");

        options
        .add_options()
        ("help", "Shows this help message.")
        ("db", "Path to the history database.", cxxopts::value<std::string>(), "DBFILE")
        ("s,session", "Creates a new session with given name and return its unique identifier.", cxxopts::value<std::string>()
         ->implicit_value("Undefined"), "SESSION_NAME")
        ("a,add", "Adds a new entry to the command history database.", cxxopts::value<std::vector<std::string>>())
        ("d,directory", "Returns commands executed in the specified directory.", cxxopts::value<std::string>()
         ->implicit_value("."), "PATH")
        ("R,recursive", "Works torgether with -d option. Changes selection to be recursive and accept all commands executed in the specified directory and all directories down by hierarchy.", cxxopts::value<bool>()->default_value("false")->implicit_value("true"))
        ("f,file", "File to parse for new entries", cxxopts::value<std::string>(), "FILE")
        ("r,separator", "Works torgether with -f option. Specifies separator between values in a file.", cxxopts::value<std::string>()
         ->implicit_value("notdefined"))
        ("v,version", "Shows version information")
        ;

        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        if (result.count("version"))
        {
            show_version(argv[0]);
            exit(0);
        }

        /*
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
        */
        
        if (!result.count("db")) {
            std::cout << options.help({""}) << std::endl;
            exit(ARG_ERROR);
        }

        History history(result["db"].as<std::string>());

        if (result.count("f")) {
            history.parse_input_file(
                result["f"].as<std::string>(),
                result["r"].as<std::string>());
        }
        else if (result.count("s")) {
            std::cout << history.insert_sess(result["s"].as<std::string>());
        }
        else if (result.count("a")) {
            auto& vals = result["a"].as<std::vector<std::string>>();

            if (vals.size() < 5) {
                std::cout << options.help({""}) << std::endl;
                exit(ARG_ERROR);
            }

            history.insert_cmd(vals[0] /*sess_id*/, vals[1] /*datetime*/, vals[2] /*pwd*/, vals[3]/*cmd*/, vals[4]/*ret_code*/);
        }
        else if (result.count("d")) {
            history.select_by_dir(
                result["d"].as<std::string>(),
                result["R"].as<bool>());
        }

    }
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        exit(ARG_ERROR);
    }
    catch (SqliteException& e)
    {
        std::cout << "Error working with database: " << e.what() << std::endl;
        exit(FAILED_TO_ADD);
    }

    return 0;
}

