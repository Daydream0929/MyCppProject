#!/bin/bash
sudo apt-get install clang-format
clang-format -style=google -dump-config > .clang-format # google style
clang-format -style=google -dump-config > .clang-format
