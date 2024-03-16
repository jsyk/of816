#!/bin/sh

# set variable X65PYHOST to the path of the x65pyhost directory
X65PYHOST=$HOME/ownCloud/elektronika/Open65/x65pyhost

echo "Reset the X65-SBC in ROMBLOCK=3 with bits 6 and 7 set to 1"
$X65PYHOST/do-cpureset.py -r 0xc3

echo "Load the forth binary to the '816 CPU bank 0x08, which coincides with ROMBLOCK=3"
$X65PYHOST/do-loadbin.py of816-x65.bin sram 0x080000

