#!/usr/bin/env bash

make

python png2bmp.py

# Run LED
sudo ./microPainter
