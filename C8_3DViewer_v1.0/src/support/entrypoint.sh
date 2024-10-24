#!/bin/bash
#cd ..

doxygen
#mkdir "latex"
cd latex/
make
#latex refman.tex
cd ..
cp -r latex/ dvi/dvi/
cp -r html/ dvi/html