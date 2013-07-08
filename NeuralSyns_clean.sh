#!/bin/sh
#
# Paulo de Castro Aguiar - Apr 2011
# pauloaguiar@fc.up.pt
#


echo ""
echo "This script cleans all object files and remove automatically produced documentation in order to reduce size and pack NeuralSyns"
echo ""


echo ""
echo "Cleaning NeuralSyns:"
echo ""
cd src
make clean
rm -f libgenerator NeuralSyns
cd ..


echo ""
echo "Cleaning NetBuilder:"
echo ""
cd tools/NetBuilder/src
make clean
rm -f NetBuilder
cd ../../..


echo ""
echo "Cleaning ElectrodeBuilder:"
echo ""
cd tools/ElectrodeBuilder/src
make clean
rm -f ElectrodeBuilder
cd ../../..


echo ""
echo "Cleaning Documentation"
echo ""
cd doc
rm -f -r latex html


echo ""
echo "All processes finished."
echo ""
