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
#include "clipp.hpp"
#include "configurator.hpp"

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


const std::string usage_header = " - history manager for Bash";

using namespace clipp;

int main(int argc, char* argv[]) {

    try {
        std::string db_path = "/not/defined.db";

        enum class mode {new_session, add, select, parse, help, version, configure};
        mode selected = mode::help;

        std::string sess_name = "notdefined";

        std::string sess_id = "0";
        std::string datetime = "1972-01-01 00:00:00";
        std::string cwd = "not defined";
        std::string cmd = "not defined";
        std::string ret_code = "0";

        bool by_dir = false;
        bool recursively = false;
        bool by_sess = false;
        std::string selection_path = ".";
        std::string session_name = "";
        
        std::string filename = "/not/defined/filename";
        std::string separator = "notdefined";

        std::string conffile = "";
        std::string hmhome = "$HOME/.hm";
        
        auto session = (
                           command("session").set(selected, mode::new_session) % "creates a new session with given name and return its unique identifier",
                           opt_value("sname", sess_name) % "name to be assigned to the created session (default: notdefined)\n"
                       );

        auto add = (
                       command("add").set(selected, mode::add) % "adds a new entry to the command history database",
                       value("sess_id", sess_id) % "session unique identifier",
                       value("datetime", datetime) % "time stamp",
                       value("cwd", cwd) % "current working directory",
                       value("cmd", cmd) % "executed command",
                       value("rc", ret_code) % "returned code\n"
                   );

        auto parse_file = (
                              command("parse").set(selected, mode::parse) % "fills database with entries from a file",
                              value("file", filename) % "file to parse",
                              value("separator", separator) % "separator which divides values in an entry\n"
                          );

        auto select = (
                          command("select").set(selected, mode::select) % "performs selection from database",
                          value("sess_id", sess_id) % "session unique identifier",
                          option("-d").set(by_dir) & opt_value("path", selection_path) % "returns commands executed in the specified directory (default: \".\")",
                          option("-R", "--recursively").set(recursively) % "works with -d option, changes selection to be recursive and accept all commands executed in the specified directory and all directories down by hierarchy",
                          option("-s").set(by_sess) & opt_value("sname", session_name) % "returns commands executed within the specified session (default: current session name)"
                      );

        auto version = (
                           option("-v", "--version").set(selected, mode::version).call(show_version) % "shows version information"
                       );

        auto help = (
                        option("-h", "--help").set(selected, mode::help) % "shows this help message\n"
                    );

        auto configure = (
                             command("configure").set(selected, mode::configure) % "starts hsitory manager configuration",
                             opt_value("file", conffile) % "file to put the configuration",
                             option("--home") & value("path", hmhome) % "sets the path to store history database (default: $HOME/.hm/)"
                         );

        auto commands = (
                            value("dbfile", db_path) % "path to the history database\n",
                            (session | add | parse_file | select)
                        );

        auto cli = (
                       (commands | version | help | configure )
                   );


        if (parse(argc, argv, cli)) {

            if (selected == mode::help) {
                std::cout << argv[0] << usage_header << std::endl << std::endl;
                std::cout << make_man_page(cli, argv[0]) << std::endl;
                exit(0);
            }

            if (selected == mode::version) {
                exit(0);
            }

            if (selected == mode::configure) {
                std::ofstream file;
                std::streambuf* output = std::cerr.rdbuf();

                if (conffile.length()) {
                    file.open(conffile, std::ios_base::app);
                    output = file.rdbuf();
                }
                
                Configurator conf(output);
                conf.configure(hmhome);
                exit(0);
            }

            History history(db_path);

            switch(selected) {
            case mode::new_session:
                std::cout << history.insert_sess(sess_name);
                break;
            case mode::add:
                history.insert_cmd(sess_id, datetime, cwd, cmd, ret_code);
                break;
            case mode::parse:
                history.parse_input_file(filename, separator);
                break;
            case mode::select:
                if (by_sess && session_name.length() == 0)
                    session_name = history.get_sess_name(sess_id);
                
                history.select(by_dir, selection_path, recursively, by_sess, session_name);
                
                if (by_sess) 
                    history.set_sess_name(sess_id, sess_name);
                break;
            default:
                std::cerr << "Error: unsupported mode" << std::endl;
                std::cerr << usage_lines(cli, argv[0]) << std::endl;
                exit(ARG_ERROR);
            }
        }
        else {
            std::cerr << "Error: failed to parse arguments" << std::endl;
            std::cerr << usage_lines(cli, argv[0]) << std::endl;
            exit(ARG_ERROR);
        }

    }
    catch (SqliteException& e) {
        std::cout << "Error: database query failed " << e.what() << std::endl;
        exit(FAILED_TO_ADD);
    }

    return 0;
}

