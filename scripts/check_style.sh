#!/usr/bin/env bash

clang-format --style=google -i src/*.cpp src/*.hpp
cpplint --recursive --exclude=src/const_conf.hpp --exclude=src/version.hpp --extensions=cpp,hpp src/
