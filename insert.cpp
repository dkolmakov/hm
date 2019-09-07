#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include "history.hpp"

namespace po = boost::program_options;

enum Type {
	ADD_SESSION = 0,
	ADD_CMD
};

enum ErrorCode {
	SUCCESS = 0,
	ARG_ERROR,
	FAILED_TO_ADD
};

void show_help(po::options_description desc) {
	std::cout << desc << std::endl; 
}

int main(int argc, char* argv[]) {
   po::options_description opts_desc("Options");

   opts_desc.add_options()
	   ("help,h", "produce help message")
	   ("type,t", po::value<int>()->required(), "type of an entry to add:\n    0 - new session, unique identifier is returned\n    1 - new command")
           ("db", po::value<std::string>()->required(), "path to the database") 
	   ("sess_name", po::value<std::string>(), "session name assigned to a created session")
	   ("sess_id", po::value<int64_t>(), "session unique identifier")
           ("pwd", po::value<std::string>(), "path to the working directory") 
           ("cmd", po::value<std::string>(), "executed command");

   po::variables_map vm; 

   try 
    { 
      po::store(po::command_line_parser(argc, argv)
		      .options(opts_desc) 
		      .run(), 
                vm); // throws on error 
 
      /** --help option 
       */ 
      if ( vm.count("help") ) 
      { 
        show_help(opts_desc);
        return SUCCESS; 
      } 
 
      po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
      
      if (vm["type"].as<int>() == ADD_CMD 
	    && (vm.count("pwd") == 0 || vm.count("cmd") == 0)) {
	     show_help(opts_desc);
	     return ARG_ERROR;
      }
    } 
    catch(boost::program_options::required_option& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return ARG_ERROR; 
    } 
    catch(boost::program_options::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return ARG_ERROR; 
    } 


   try {
	History history(vm["db"].as<std::string>());

	switch (vm["type"].as<int>()) {
		case ADD_SESSION:
			std::cout << history.insert_sess(
					vm["sess_name"].as<std::string>());
			break;
		case ADD_CMD:
			history.insert_cmd(
					vm["sess_id"].as<int64_t>(),
					vm["pwd"].as<std::string>(), 
					vm["cmd"].as<std::string>());
			break;

	}

   }
   catch (int e) 
   {
	   return FAILED_TO_ADD;
   }

   return 0;
}
