
#pragma once

#include <string>
#include <iostream>

template<class T>
struct ProgressBar {
    const T total;
    size_t bar_width;
        
    ProgressBar(T _total, T current, size_t _bar_width) : total(_total), bar_width(_bar_width) {
        show_progress(current);
    }

    ~ProgressBar() {
    }
    
    void show_progress(T current) {
        double progress = current * 1.0 / total;
        std::cout << "[";
        size_t pos = bar_width * progress;
        for (size_t i = 0; i < bar_width; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] ";
        
        int percents = int(progress * 100.0);
        if (percents < 100) { 
            std::cout << percents << " %\r";
        }
        else {
            std::cout << " Done!" << std::endl;
        }
        
        std::cout.flush();
    }   
};

