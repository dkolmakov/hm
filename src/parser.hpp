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

#include <iostream>
#include <string>

#include "./clipp.hpp"
#include "./utils.hpp"
#include "./version.hpp"

using clipp::command;
using clipp::doc_formatting;
using clipp::group;
using clipp::opt_value;
using clipp::option;
using clipp::parameter;
using clipp::usage_lines;
using clipp::value;

struct AgrgumentException : UtilException {
  explicit AgrgumentException(const std::string& _reason)
      : UtilException("Argument parsing error: ", _reason) {}
};

enum class mode {
  new_session,
  add,
  select,
  parse,
  help,
  wrapped_help,
  version,
  configure,
  info
};

class ArgParser {
  static void show_version(std::string name) {
    std::cout
        << name << " " << HM_VERSION << std::endl
        << "Copyright (C) 2020 Dmitry Kolmakov." << std::endl
        << "License ASLv2.0: Apache License, Version 2.0 "
           "<http://www.apache.org/licenses/LICENSE-2.0>."
        << std::endl
        << "This is free software: you are free to change and redistribute it."
        << std::endl
        << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
  }
  const parameter version = (option("-v", "--version")
                                 .set(selected, mode::version)
                                 .call(show_version) %
                             "shows version information");

 public:
  std::string db_path = "/not/defined.db";

  mode selected = mode::help;

  // session command
  std::string session_name = "";
  const group session =
      (command("session").set(selected, mode::new_session),
       "session - creates a new session and return its unique identifier" %
           group(opt_value("sname", session_name) %
                 "name to be assigned to the created session (default: "
                 "notdefined)"));

  // add command
  std::string sess_id = "0";
  std::string datetime = "1972-01-01 00:00:00";
  std::string cwd = "not defined";
  std::string cmd = "not defined";
  std::string ret_code = "0";
  const group add =
      (command("add").set(selected, mode::add),
       "add - adds a new entry to the command history database" %
           (value("sess_id", sess_id) % "session unique identifier",
            value("datetime", datetime) % "time stamp",
            value("cwd", cwd) % "current working directory",
            value("cmd", cmd) % "executed command",
            value("rc", ret_code) % "returned code"));

  // select command
  bool by_dir = false;
  bool recursively = false;
  bool by_sess = false;
  std::string selection_path = ".";
  std::string selection_session_name = "";
  const group select_options =
      (opt_value("path", selection_path).set(by_dir) %
           "returns commands executed in the specified directory (default: "
           "\".\")",
       option("-R", "--recursively").set(recursively) %
           "works with -d option, changes selection to be recursive and accept "
           "all commands executed in the specified directory and all "
           "directories down by hierarchy",
       option("-s").set(by_sess) &
           opt_value("sname", selection_session_name) %
               "returns commands executed within the specified session "
               "(default: current session name)");
  const group select =
      (command("select").set(selected, mode::select),
       "select - performs selection from database" %
           (value("sess_id", sess_id) % "session unique identifier",
            select_options));

  // parse command
  std::string filename = "/not/defined/filename";
  std::string separator = "notdefined";
  const group parse_file =
      (command("parse").set(selected, mode::parse),
       "parse - fills database with entries from a file" %
           (value("file", filename) % "file to parse",
            value("separator", separator) %
                "separator which divides values in an entry"));

  const group info =
      (command("info").set(selected, mode::info),
       "info - outputs the session info" %
           group(value("sess_id", sess_id) % "session unique identifier"));

  // configure command
  std::string conffile = "";
  std::string hmhome = "$HOME/.hm";
  const group configure =
      (command("configure").set(selected, mode::configure),
       "configure - performs history manager configuration" %
           (opt_value("file", conffile) % "file to put the configuration",
            option("--home") & value("path", hmhome) %
                                   "sets the path to store history "
                                   "database (default: $HOME/.hm/)"));

  const parameter help = (option("-h", "--help").set(selected, mode::help) %
                          "shows this help message");

  const parameter wrapped_help =
      (option("--wrapped-help").set(selected, mode::wrapped_help) %
       "shows help message for a wrapped application, where <db-file> is "
       "determined automatically\n");

  const group commands =
      (value("dbfile", db_path) % "path to the history database\n",
       (session | add | parse_file | select | info));

  const group cli = ((commands | version | help | wrapped_help | configure));

  const std::string program_name;

  void print_usage() const {
    std::cerr << usage_lines(cli, program_name) << std::endl;
  }

  void parse(int argc, char* argv[]) const {
    if (!clipp::parse(argc, argv, cli)) {
      throw AgrgumentException("failed to parse arguments");
    }

    const auto clipp_fmt =
        doc_formatting{}.first_column(4).doc_column(28).last_column(80);

    if (selected == mode::help) {
      std::cout << program_name << " - history manager for Bash" << std::endl
                << std::endl;
      std::cout << make_man_page(cli, program_name, clipp_fmt) << std::endl;
    }
  }

  explicit ArgParser(const std::string& pname) noexcept : program_name(pname) {}
};

struct WrappedArgsParser {
  const std::string program_name;
  const group selection_options;

  const parameter version =
      (option("-v", "--version") % "shows version information");

  const parameter help = (option("-h", "--help") % "shows this help message");

  const parameter info =
      (option("-i", "--info") % "outputs the current session info");

  const group wrapper_cli = (help | version | info | selection_options);

  void print_help() const {
    const auto clipp_fmt =
        doc_formatting{}.first_column(4).doc_column(28).last_column(80);

    std::cout << program_name
              << " - history manager command line wrapper, which eases access "
                 "to hm-db application"
              << std::endl
              << std::endl;
    std::cout << make_man_page(wrapper_cli, program_name, clipp_fmt)
              << std::endl;
  }

  WrappedArgsParser(const std::string& pname, const group& sel) noexcept
      : program_name(pname), selection_options(sel) {}
};
