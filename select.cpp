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
#include <assert.h>

#include <string>
#include <iostream>

#include "history.hpp"

void replace_all(std::string& str, const std::string& from, const std::string& to) {
    size_t pos = 0;

    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

int main(int argc, char* argv[]) {
    assert(argc == 3);

    std::string db_path = argv[1];
    std::string dir = argv[2];

    History history(db_path);
    history.select_by_dir(dir);

    return 0;
}

