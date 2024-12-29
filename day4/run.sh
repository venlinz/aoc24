#!/bin/bash
set -xe

clear
gcc-14 solution.c -Wextra -Wall -pedantic -Werror
./a.out
