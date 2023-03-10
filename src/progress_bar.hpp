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

#include <sys/ioctl.h>

#include <iostream>
#include <string>

template <class T>
struct ProgressBar {
  const T total;
  size_t bar_width;
  const char* done = "Done!";

  ProgressBar(T _total, T current) : total(_total) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    bar_width = w.ws_col - strlen(done) - 3;  // 1 for "[" + 2 for "] "

    show_progress(current);
  }

  ~ProgressBar() {}

  void show_progress(T current) {
    double progress = current * 1.0 / total;
    std::cout << "[";
    size_t pos = bar_width * progress;

    for (size_t i = 0; i < bar_width; ++i) {
      if (i < pos)
        std::cout << "=";
      else if (i == pos)
        std::cout << ">";
      else
        std::cout << " ";
    }
    std::cout << "] ";

    int percents = static_cast<int>(progress * 100.0);
    if (percents < 100) {
      std::cout << percents << "%\r";
    } else {
      std::cout << done << std::endl;
    }

    std::cout.flush();
  }
};
