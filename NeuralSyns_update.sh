#!/bin/sh
#
# Paulo de Castro Aguiar - Apr 2011
# pauloaguiar@fc.up.pt
#


echo ""
echo "This script recompile NeuralSyns if new models have been added or changes have been made in the source code."
echo "The updated binary will be placed in your ~/bin directory."
echo ""


echo ""
echo "Compiling NeuralSyns:"
echo ""
cd src
make clean #the dependencies are complex and the compilation is extremelly fast... this is a quick and dirty solution
make install

make clean #repeat for the MC version
make -f Makefile_MC install
cd ..


echo ""
echo "All processes finished."
echo ""
