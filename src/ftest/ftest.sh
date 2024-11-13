#!/bin/sh
# Este script ejecuta los tests funcionales del programa principal
# está preparado para poder ejecutarse directamente con sbatch

python3 ftest.py
rm -rf *.ppm
rm -rf *.cppm