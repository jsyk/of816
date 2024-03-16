#!/bin/bash
set -e -x
cd $(dirname $0)
ca65 -I ../../inc x65.s -l x65.lst
ca65 -I ../../inc romboot.s -l romboot.lst
../../build.sh X65
ld65 -vm -C x65.l x65.o ../../forth.o ./romboot.o -m forth.map -o of816-x65.bin
ls -l of816-x65.bin
# if which -s bin2hex; then
#   bin2hex of816-x65.bin > of816-x65.hex
#   ls -l of816-x65.hex
# fi

