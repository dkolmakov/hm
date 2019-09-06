#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <boost/program_options.hpp>

#include "history.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
   po::options_description opts_desc("Options");

   opts_desc.add_options()
	   ("help", "produce help message")
           ("db_path", po::value<std::string>()->required(), "path to the database") 
           ("pwd", po::value<std::string>()->required(), "path to the working directory") 
           ("cmd", po::value<std::string>()->required(), "executed command");

   po::positional_options_description pos_opts_desc;
   pos_opts_desc.add("db_path", 1);
   pos_opts_desc.add("pwd", 1);
   pos_opts_desc.add("cmd", 1);

   po::variables_map vm; 

   try 
    { 
      po::store(po::command_line_parser(argc, argv)
		      .options(opts_desc) 
                      .positional(pos_opts_desc)
		      .run(), 
                vm); // throws on error 
 
      /** --help option 
       */ 
      if ( vm.count("help") ) 
      { 
	std::cout << opts_desc << std::endl; 
        return 0; 
      } 
 
      po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    } 
    catch(boost::program_options::required_option& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return -1; 
    } 
    catch(boost::program_options::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      return -2; 
    } 

   try {
   	History history(vm["db_path"].as<std::string>());
   	history.insert(vm["pwd"].as<std::string>(), vm["cmd"].as<std::string>());
   }
   catch (int e) 
   {
	   return e;
   }

   return 0;
}
