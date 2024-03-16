#!/bin/sh

# set variable X65PYHOST to the path of the x65pyhost directory
X65PYHOST=$HOME/ownCloud/elektronika/Open65/x65pyhost

# execute do-loadbin.py to load the forth binary to the bank $08 of the connected hw
$X65PYHOST/do-loadbin.py of816-x65.bin sram 0x080000

echo "Now reset the X65-SBC in ROMBLOCK=3"
