Thank you for your interest in NeuralSyns!

NeuralSyns allows construction, simulation and visualization of spiking networks. New neuron and synapse dynamics can be added, and complex network models can be easily built using the included tool NetBuilder. OpenGL is used to show the network's architecture and activity.

In order to have NeuralSyns running in your system you need to follow these steps.
Send any questions to pauloaguiar@fc.up.pt


STEP 1
======

First you need the following libraries to be installed in your gnu-linux system (you may have some, if not all, already installed):
- gsl (libgsl-dev, libgsl0-dev) : GNU Scientific Library, GSL
- freeglut (freeglut3-dev) : an opensource alternative to the OpenGL Utility Toolkit
- wxWidgets 2.8 ( libwxbase2.8-dev, wx-common, libwxgtk2.8-dev ) : wxWidgets cross-platform C++ GUI toolkit (GTK+ development)
- gtk2.0 ( libgtk2.0-dev, libgtk2.0-common ) : toolkit for creating graphical user interfaces
- python3.0 ( python-all-dev - python3-all-dev, libpython3.0,  libpython3.1) : Python

If you intend to use matplotlib together with python you also need:
- matplotlib : python based plotting system in a style similar to Matlab
- python-numpy : numerical Python adds a fast array facility to the Python language
- python-gtk2-dev : python GTK+ bindings, development files

If you want to use the pyNet2NeuroML functionality:
- PyXB : a Python package that generates source code for classes corresponding to data structures defined by XMLSchema

Please notice that all these are well known, standard libraries and many linux distributions include them in their repositories, making the installation procedure totally straightforward. On Ubuntu you may simply use Synaptic and select the above mentioned packages.


STEP 2
======

Now that you have all the required libraries installed, you are ready to compile NeuralSyns in your system. Download the latest version of NeuralSyns from: http://sourceforge.net/projects/neuralsyns.
Extract the contents of the zip file to your home directory (~/NeuralSyns).
To install NeuralSyns go to ~/NeuralSyns and run the shell script "NeuralSyns_install":

$ unzip NeuralSyns.zip -d ~/
$ cd ~/NeuralSyns
$ chmod +x NeuralSyns_install
$ ./NeuralSyns_install

Running the "NeuralSyns_install" script will compile and link all source files and place three binaries in your ~/NeuralSyns/bin directory: NeuralSyns, NetBuilder and ElectrodeBuilder. If you have correctly installed all the required libraries, you should have no errors, and the compilation takes just a few seconds. If you double checked everything (especially you confirmed that all the required libraries have been installed!) and you still are not able to compile NeuralSyns, send an email to me: pauloaguiar@fc.up.pt


STEP 3
======

Next include ~/NeuralSyns/bin in your $PATH environment variable. Append the following line to your ~/.bashrc, ~/.bash_profile or ~/.profile file (depends on your distribution setup,  on Ubuntu append the ~/.bashrc file):

export PATH=$PATH:~/NeuralSyns/bin 


STEP 4 
======

For NeuralSyns documentation go to ~/NeuralSyns/doc. You can generate doxygen documentation regarding NeuralSyns code: simply type "doxygen" while inside ~/NeuralSyns/doc,  an html and a latex version will be created in different directories. A great deal of care has been taken in commenting the source code. We therefore expect the doxygen output to be useful.




Hope you enjoy NeuralSyns!

Paulo de Castro Aguiar - pauloaguiar@fc.up.pt
Centro de Matematica da Universidade do Porto, Portugal

Porto, Dec 2012
