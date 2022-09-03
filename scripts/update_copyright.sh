#!/usr/bin/env bash
current_year=$(date +"%Y")
find $1 -name "*.cpp" -o -name "*.hpp" -print0 | xargs -0 \
sed -i -E "s/(Copyright 2019).*( Dmitry Kolmakov)/\1-${current_year}\2/"
