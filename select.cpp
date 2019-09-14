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

    replace_all(dir, "/", " ");

    History history(db_path);
    history.select_by_dir(dir);

    return 0;
}

