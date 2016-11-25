#!/usr/bin/env bash

# Make files
make all

# Convert png to bmp
python png2bmp.py

# Run LED
./dotStar