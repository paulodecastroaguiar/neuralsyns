#!/bin/sh
#
# Paulo de Castro Aguiar - Apr 2011
# pauloaguiar@fc.up.pt
#


echo ""
echo "This script compiles NeuralSyns together with NetBuilder and ElectrodeBuilder. It also handles the pyEditNet python script."
echo "All three binaries and the script will be placed in your ~/bin directory."
echo ""


echo ""
echo "Compiling NeuralSyns:"
echo "====================="
echo ""
cd src
make clean
make
make install
cd ..


echo ""
echo "Compiling NeuralSyns - MultiCore version:"
echo "========================================="
echo ""
cd src
make -f Makefile_MC clean
make -f Makefile_MC
make -f Makefile_MC install
cd ..


echo ""
echo "Compiling NetBuilder:"
echo "====================="
echo ""
cd tools/NetBuilder/src
make clean
make
make install
cd ../../..


echo ""
echo "Compiling ElectrodeBuilder:"
echo "==========================="
echo ""
cd tools/ElectrodeBuilder/src
make clean
make
make install
cd ../../..


echo ""
echo "Transfering pyEditNet:"
echo "======================"
echo ""
cp ~/NeuralSyns/tools/PyEditNet/pyEditNet.sh ~/NeuralSyns/bin/
chmod +x ~/NeuralSyns/bin/pyEditNet.sh
cp ~/NeuralSyns/tools/PyEditNet/pyNet2NeuroML.sh ~/NeuralSyns/bin/
chmod +x ~/NeuralSyns/bin/pyNet2NeuroML.sh
echo "A copy of 'pyEditNet' and 'pyNet2NeuroML' were placed in your ~/NeuralSyns/bin/ directory.
"

echo ""
echo "Documentation:"
echo "=============="
echo ""
echo "To generate documentation for NeuralSyns run the command 'doxygen' inside the ~/NeuralSyns/doc directory."
#cd doc
#doxygen
#cd latex
#pdflatex refman.tex
#pdflatex refman.tex
#cd ../..




echo ""
echo "All processes finished."
echo ""
echo "Make sure ~/NeuralSyns/bin directory is part of your PATH variable."