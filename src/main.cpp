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
#include "parser.hpp"

enum ErrorCode {
    SUCCESS = 0,
    ARG_ERROR,
    FAILED_TO_ADD
};

static void print_session_info(std::string sess_id, std::string sess_name)
{
    std::cout << "Session #" << sess_id << " with name: " << sess_name << std::endl;
}

int main(int argc, char* argv[]) {
    ArgParser args("hm-db");

    try {
        args.parse(argc, argv);

        if (args.selected == mode::help || args.selected == mode::version) {
            exit(0);
        }
        
        if (args.selected == mode::wrapped_help) {
            WrappedArgsParser wrappedArgs("hm", args.select_options);
            wrappedArgs.print_help();
            exit(0);
        }

        if (args.selected == mode::configure) {
            std::ofstream file;
            std::streambuf* output = std::cerr.rdbuf();

            if (args.conffile.length()) {
                file.open(args.conffile, std::ios_base::app);
                output = file.rdbuf();
            }
            
            Configurator conf(output);
            conf.configure(args.hmhome);
            exit(0);
        }

        History history(args.db_path);

        switch(args.selected) {
        case mode::new_session:
            std::cout << history.insert_sess(args.sess_name);
            break;
        case mode::add:
            history.insert_cmd(args.sess_id, args.datetime, args.cwd, args.cmd, args.ret_code);
            break;
        case mode::parse:
            history.parse_input_file(args.filename, args.separator);
            break;
        case mode::select:
            if (args.by_sess && args.session_name.length() == 0)
                args.session_name = history.get_sess_name(args.sess_id);
            
            history.select(args.by_dir, args.selection_path, args.recursively, args.by_sess, args.session_name);
            
            if (args.by_sess) 
                history.set_sess_name(args.sess_id, args.sess_name);
            break;
        case mode::info:
            args.sess_name = history.get_sess_name(args.sess_id); 
            print_session_info(args.sess_id, args.sess_name);
            break;
        default:
            throw AgrgumentException("Error: unsupported mode");
        }
    }
    catch (SqliteDB::SqliteException& e) {
        std::cout << "Error: database query failed " << e.what() << std::endl;
        exit(FAILED_TO_ADD);
    }
    catch (AgrgumentException& e) {
        std::cout << "Error: " << e.what() << std::endl;
        args.print_usage();
        exit(ARG_ERROR);
    }

    return 0;
}

