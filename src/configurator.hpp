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

#include <exception>
#include <iostream>
#include <string>

#include "const_conf.hpp"

struct ConfException : public std::exception {
    std::string reason;

    ConfException(std::string _reason) {
        reason = "Configurator error: " + _reason;
    }

    const char * what () const throw () {
        return reason.c_str();
    }
};


class Configurator {

    std::string configuration = "";
    std::ostream output;

public:

    Configurator(std::streambuf* out_buf) : output(out_buf) {
    }

    int configure(const std::string hmhome) {
        std::cout << "Starting history manager configuration" << std::endl;

        configure_home_path(hmhome);
        print_configuaration();

        std::cout << "Done." << std::endl;
        return 0;
    }

    void configure_home_path(const std::string hmhome) {
        configuration += "# History manager home directory\n";
        configuration += "hm_home=" + hmhome + "\n\n";
    }

    void print_configuaration() {
        output << configuration;
        output << const_configuration;
    }

    ~Configurator() {
    }
};
