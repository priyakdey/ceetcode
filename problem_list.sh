#!/bin/bash

set -xe

python3 problem_list.py > problem_list.temp

sed -i '/<!-- PROBLEM_LIST:START -->/,/<!-- PROBLEM_LIST:END -->/d' README.md
echo '<!-- PROBLEM_LIST:START -->' >> README.md
cat problem_list.temp >> README.md 
echo '<!-- PROBLEM_LIST:END-->' >> README.md
rm problem_list.temp
