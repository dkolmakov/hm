/*
   Copyright 2019-2023 Dmitry Kolmakov

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

#include "./const_conf.hpp"

class Configurator {
  std::string configuration = "";
  std::ostream output;

 public:
  explicit Configurator(std::streambuf* out_buf) : output(out_buf) {}

  int configure(const std::string hmhome) {
    configure_home_path(hmhome);
    print_configuaration();
    return 0;
  }

  void configure_home_path(const std::string hmhome) {
    configuration += "\t# History manager home directory\n";
    configuration += "\thm_home=" + hmhome + "\n\n";
  }

  void print_configuaration() {
    output << "\n# History manager settings\n";
    output << "if command -v hm-db > /dev/null && [ $(id -u) -ne 0 ] && [ -z "
              "$hm_home ]\nthen\n";

    output << configuration;
    output << const_configuration;

    output << "fi\n" << std::endl;
  }

  ~Configurator() {}
};
