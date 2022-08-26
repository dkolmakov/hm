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
#include <string>

struct UtilException : public std::exception {
    const std::string reason;

    UtilException(const std::string& _title, const std::string& _reason, const int rc) :
        reason(_title + std::to_string(rc) + ": " + _reason) {}

    UtilException(const std::string& _title, const std::string& _reason) :
        reason(_title + ": " + _reason) {}
        
    const char * what () const throw () {
        return reason.c_str();
    }
};

