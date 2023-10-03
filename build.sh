#!/bin/bash
sudo apt-get install clang-format
clang-format -style=google -dump-config > .clang-format # google style
clang-format -style=google -dump-config > .clang-format

#
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format -style=file -i {} \;

