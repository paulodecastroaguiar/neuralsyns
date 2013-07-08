#-*- coding: utf-8 -*-
"""
 @file Py_Edit_Network.py
 @author Mafalda Sousa and Paulo Aguiar

 These functions allows manipulation of the network model, by reading all data from .net file created by Netbuilder, editing with python functions and writing again in a .net file format
"""

import sys
import string
import os.path
import stat
import inspect
import math
import random
import time
from Py_Structures import *
from math import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
      



###############################################################################################################

def main():

	if len(sys.argv) < 2 :
		print "\nERROR: Name of .net file missing.\nRe-run with the network file you want to load and manipulate.\nExample:\n\npyNetBuilder.sh mynetworkfile.net\n\n"
		os._exit(0)
	else:
		network_file = sys.argv[1]
		Read_Network(network_file) 	#Read the network file and store data in corresponding structures

		print "\nManipulate the network model using inbuilt functions or load your own python scripts. For a list of available functions type 'info()'.\n\nWhen you are done use the function Write_Network('networkfilename.net') to save your modified network model.\n"



###############################################################################################################



## {{{ http://code.activestate.com/recipes/168639/ (r1)
class progressBar:
	def __init__(self, minValue = 0, maxValue = 10, totalWidth=12):
		self.progBar = "[]"   # This holds the progress bar string
		self.min = minValue
		self.max = maxValue
		self.span = maxValue - minValue
		self.width = totalWidth
		self.amount = 0       # When amount == max, we are 100% done 
		self.updateAmount(0)  # Build progress bar string

	def updateAmount(self, newAmount = 0):
		if newAmount < self.min: newAmount = self.min
		if newAmount > self.max: newAmount = self.max
		self.amount = newAmount

		# Figure out the new percent done, round to an integer
		diffFromMin = float(self.amount - self.min)
		percentDone = (diffFromMin / float(self.span)) * 100.0
		percentDone = round(percentDone)
		percentDone = int(percentDone)

		# Figure out how many hash bars the percentage should be
		allFull = self.width - 2
		numHashes = (percentDone / 100.0) * allFull
		numHashes = int(round(numHashes))

		# build a progress bar with hashes and spaces
		self.progBar = "[" + '#'*numHashes + ' '*(allFull-numHashes) + "]"

		# figure out where to put the percentage, roughly centered
		percentPlace = (len(self.progBar) / 2) - len(str(percentDone)) 
		percentString = str(percentDone) + "%"

		# slice the percentage into the bar
		self.progBar = self.progBar[0:percentPlace] + percentString + self.progBar[percentPlace+len(percentString):]

	def __str__(self):
		return str(self.progBar)
## end of http://code.activestate.com/recipes/168639/ }}}



###############################################################################################################



def info():
	#Prints all the functions information
	filename = os.path.expanduser("~/NeuralSyns/tools/PyEditNet/info.txt")
	if os.path.isfile ( filename )==False:
		print('\nERROR: unable to read info.txt file')
		os._exit(0)

	inp = open( filename, 'r')
	while True:
		temp_line = inp.readline()
		if len(temp_line) ==0:
		    break # EOF
		else: 
		    linelist = string.split(temp_line)
		    try:
			linelist[0]=int(linelist[0])
			print "\033[1m",temp_line[2:],"\033[0m"
		    except:
			print temp_line
	
	inp.close()



###############################################################################################################



def Check_data_type(data,data_type,line):
	"""Checks if variables have the correct data type
        @param data variable to test	
	@param data_type type of variable	
	@param line which line is the error, if it exists
        @return error if invalid data type is find"""
	
	if type(data).__name__!=data_type:
		print '\nERROR: Invalid data type at %s:%d' % line
		os._exit(0)



###############################################################################################################



def Read_Network(network_file):
	"""Reads all data from the network model data file
        @param *network_file network model file name 
        @return Finished reading file if the process goes well"""	
	
	t = time.clock()
	# test if the file exists
	filename = network_file
	if os.path.isfile ( filename )==False:
		print('\nERROR: unable to read neurons _network_ data file')
		os._exit(0)
		return None; 

	inp = open(network_file, 'r') #open _network data file


	print '\nReading network file: ', network_file

	#Skip all comment lines. Begin at START keyword
	temp_line=inp.readline()
	while temp_line!='START\n':
		temp_line = inp.readline()
		if temp_line == '' :
			print '\n\tERROR: the keyword START was not found in _network_ file\n\n'
			inp.close()
			exit(-1)
    


        # Start taking the data
        #--------------------------------------------------------------------

	# Models
  
	inp.readline()

	linestring=inp.readline()
	linelist = string.split(linestring)

	global total_models
	total_models=int(linelist[1])
	
	global models
	models=[]
	
	global temp_list
	temp_list={}
	for m in range(total_models):
	    linestring=inp.readline()
	    linelist = string.split(linestring)
	    models.append(MODEL_DYNAMICS())
	    models[m].model_label= linelist[0]
	    temp_list[linelist[0]]=m
	    models[m].n_statevars = int(linelist[1])
	    models[m].statevar_label=[0]*models[m].n_statevars
	    for var in range(models[m].n_statevars):
	      models[m].statevar_label[var]=linelist[var+2]
	    models[m].n_params = int(linelist[models[m].n_statevars+2])
	    models[m].param_label=[0]*models[m].n_params
	    for par in range(models[m].n_params):
	      models[m].param_label[par]=linelist[par+models[m].n_statevars+3]
	

         #POPULATIONS total_populations


	inp.readline()

	linestring=inp.readline()
	linelist = string.split(linestring)

        global total_populations
        total_populations=int(linelist[1])

        print 'Populations = ', total_populations

        global population
	population=[]                             #list/array of struct population

	for pop in range( total_populations):
		population.append(POPULATION())   #fill with default values (malloc and init)
		
	linestring=inp.readline()
	linelist = string.split(linestring)
	
	for pop in range(total_populations):
		population[pop].label=linelist[pop+2]		#population label
	
	linestring=inp.readline()
	linelist = string.split(linestring)
	first_neuron=0
	for pop in range(total_populations):
		population[pop].n_neurons=int(linelist[pop+2])	#population size
		if (pop==0):
		    population[pop].first_neuron=0
		else :
		    first_neuron=population[pop-1].n_neurons+first_neuron
		    population[pop].first_neuron=first_neuron	#population first_neuron

	linestring=inp.readline()
	linelist = string.split(linestring)

	for pop in range(total_populations):
		population[pop].n_fdomains=int(linelist[pop+2])	#population number of fdomains


	#FDOMAINS

	inp.readline()
	inp.readline()

	for pop in range( total_populations):
		linestring=inp.readline()
		linelist = string.split(linestring)
		population[pop].fdomain=[]
                for fdm in range (population[pop].n_fdomains):
                    population[pop].fdomain.append(FDOMAIN_GLOBAL())
                    population[pop].fdomain[fdm].fdm_ID=int(linelist[7*fdm+2])
                    population[pop].fdomain[fdm].label=linelist[7*fdm+3]
                    population[pop].fdomain[fdm].parent_fdomain=int(linelist[7*fdm+4])
                    population[pop].fdomain[fdm].position=POSITION()
                    topology=linelist[7*fdm+5]
                    position=topology.split(',')
                    population[pop].fdomain[fdm].position.x=position[0][1:]
                    population[pop].fdomain[fdm].position.y=position[1]
                    population[pop].fdomain[fdm].position.z=position[2]
                    population[pop].fdomain[fdm].position.r=position[3][:-1]
                    population[pop].fdomain[fdm].model= models[temp_list[linelist[7*fdm+6]]]
		    
   
	# Homogeneity

	inp.readline()

	linestring=inp.readline()
	linelist = string.split(linestring)
	
	global homogenity
	homogenity=linelist[2]
        
	#Connection pairs
	inp.readline()
	
	linestring=inp.readline()
	linelist = string.split(linestring)
	global total_connection_pairs
	total_connection_pairs=int(linelist[2])

	global connection
	connection=[0]*total_populations
	for pop in range(total_populations):
	  connection[pop]=[0]*total_populations


	for pairs in range (total_connection_pairs):
		linestring=inp.readline()
		linelist = string.split(linestring)

		pop_pre=int(linelist[8])
		pop_pos=int(linelist[9])
		connection[pop_pre][pop_pos]=CONNECTION()
		connection[pop_pre][pop_pos].label=linelist[3]+">>"+linelist[4]
		connection[pop_pre][pop_pos].fdomain=int(linelist[11])

		linestring=inp.readline()
		linelist = string.split(linestring)
		connection[pop_pre][pop_pos].current_model=models[temp_list[linelist[0]]]
		if homogenity == 'YES':
			connection[pop_pre][pop_pos].current_param=[0]*connection[pop_pre][pop_pos].current_model.n_params
			for par in range (connection[pop_pre][pop_pos].current_model.n_params):
				connection[pop_pre][pop_pos].current_param[par]=float(linelist[par+1])

 		linestring=inp.readline()
		linelist = string.split(linestring)
		connection[pop_pre][pop_pos].plasticity_model=models[temp_list[linelist[0]]]
		if homogenity == 'YES':
			connection[pop_pre][pop_pos].plasticity_param=[0]*connection[pop_pre][pop_pos].plasticity_model.n_params
			for par in range(connection[pop_pre][pop_pos].plasticity_model.n_params):
				connection[pop_pre][pop_pos].plasticity_param[par]=float(linelist[par+1])

 	#Neurons
	inp.readline()
	linestring=inp.readline()
	linelist = string.split(linestring)
        global total_neurons
	total_neurons=int(linelist[1])

	linestring=inp.readline()
	linelist = string.split(linestring)
        global total_synapses
	total_synapses=int(linelist[1])

	inp.readline()

	print 'Neurons = ', total_neurons
	print 'Synapses = ', total_synapses
	print ''
	
        global neuron
	neuron=[]
	pop=0
	temp_nrn_sum = population[pop].n_neurons;
	
	prog = progressBar(0, total_neurons, 77)

	for nrn in range (total_neurons):
		if (nrn%10==0):
		    prog.updateAmount(nrn)
		    print prog, "\r",
		
		linestring=inp.readline()
		linelist = string.split(linestring)

		neuron.append(NEURON())

		linestring=inp.readline()
		linelist = string.split(linestring)
		neuron[nrn].position = POSITION()
		neuron[nrn].position.x = float(linelist[0][1:-1])
		neuron[nrn].position.y = float(linelist[1][0:-1])
		neuron[nrn].position.z = float(linelist[2][0:-1])
		
                #check to which population this neuron belongs
		if( nrn > temp_nrn_sum - 1 ):
			pop=pop+1
			temp_nrn_sum =	temp_nrn_sum + population[pop].n_neurons;
			
		neuron[nrn].population = pop			#ID of neuron's host population

		neuron[nrn].n_fdomains=population[pop].n_fdomains
		neuron[nrn].fdomain=[]
		
		
		for fdm in range(neuron[nrn].n_fdomains):
			linestring=inp.readline()
			linelist = string.split(linestring)
			neuron[nrn].fdomain.append(FDOMAIN())
			neuron[nrn].fdomain[fdm].Ra=float(linelist[0])
			neuron[nrn].fdomain[fdm].V=float(linelist[1])
			n_statevars=int(population[pop].fdomain[fdm].model.n_statevars)
			neuron[nrn].fdomain[fdm].model_statevar=[0]*n_statevars
			for var in range (n_statevars):
				neuron[nrn].fdomain[fdm].model_statevar[var]=float(linelist[var+2])
			
			total_params=int(population[pop].fdomain[fdm].model.n_params)
			neuron[nrn].fdomain[fdm].model_param=[0]*total_params
			for par in range (total_params):
				neuron[nrn].fdomain[fdm].model_param[par]=float(linelist[par+2+n_statevars])
				
		linestring=inp.readline()
		linelist = string.split(linestring)
		neuron[nrn].n_spike_times = int(linelist[1])
		neuron[nrn].spike_time=[0]*neuron[nrn].n_spike_times
		for spk in range(neuron[nrn].n_spike_times):
		  neuron[nrn].spike_time[spk] = float(linelist[spk+2]);

		linestring=inp.readline()
		linelist = string.split(linestring)
		neuron[nrn].n_synapses = int(linelist[1]);
		neuron[nrn].synapse=[]
		for syn in range (neuron[nrn].n_synapses):
			linestring=inp.readline()
			linelist = string.split(linestring)
			neuron[nrn].synapse.append(SYNAPSES())
			neuron[nrn].synapse[syn].target_neuron=int(linelist[1])
			# find out the neuron's target population
			pop_target = 0
			for p in range(1,total_populations):
			  if (neuron[nrn].synapse[syn].target_neuron<population[p].first_neuron):
			    pop_target = int(p-1)
			    break
			  elif (neuron[nrn].synapse[syn].target_neuron>=population[p].first_neuron):
			    pop_target = int(p)
			    
			neuron[nrn].synapse[syn].weight=float(linelist[2])
			neuron[nrn].synapse[syn].delay=float(linelist[3])			

			linestring=inp.readline()
			linelist = string.split(linestring)
			neuron[nrn].synapse[syn].spikes_cue=int(linelist[0])
			neuron[nrn].synapse[syn].activated=int(linelist[1])
			neuron[nrn].synapse[syn].psr_time=float(linelist[2])

			linestring=inp.readline()
			linelist = string.split(linestring)
			neuron[nrn].synapse[syn].current_statevar=[0]*connection[neuron[nrn].population][pop_target].current_model.n_statevars
			for var in range (connection[neuron[nrn].population][pop_target].current_model.n_statevars):
			    neuron[nrn].synapse[syn].current_statevar[var]=float(linelist[var+1])
			if homogenity=='YES':
			    neuron[nrn].synapse[syn].current_param=connection[neuron[nrn].population][pop_target].current_param
			if homogenity=='NO':				
			    neuron[nrn].synapse[syn].current_param=[0]*connection[neuron[nrn].population][pop_target].current_model.n_params
			    for par in range (connection[neuron[nrn].population][pop_target].current_model.n_params):
				neuron[nrn].synapse[syn].current_param[par]=float(linelist[par+1+connection[neuron[nrn].population][pop_target].current_model.n_statevars])
				
			linestring=inp.readline()
			linelist = string.split(linestring)

			neuron[nrn].synapse[syn].plasticity_statevar=[0]*connection[neuron[nrn].population][pop_target].plasticity_model.n_statevars
			for var in range (connection[neuron[nrn].population][pop_target].plasticity_model.n_statevars):
			    neuron[nrn].synapse[syn].plasticity_statevar[var]=float(linelist[var+1])
			if homogenity=='YES':	
			    neuron[nrn].synapse[syn].plasticity_param=connection[neuron[nrn].population][pop_target].plasticity_param	
			if homogenity=='NO':
				neuron[nrn].synapse[syn].plasticity_param=[0]*connection[neuron[nrn].population][pop_target].plasticity_model.n_params
				for par in range(connection[neuron[nrn].population][pop_target].plasticity_model.n_params):
					neuron[nrn].synapse[syn].plasticity_param[par]=float(linelist[par+1+connection[neuron[nrn].population][pop_target].plasticity_model.n_statevars])
			inp.readline()
		if (neuron[nrn].n_synapses==0):
		    inp.readline()
	print '\nFinished reading file. Elapsed time = ', time.clock()-t, 'secs.\n'
	inp.close()



###############################################################################################################



def Write_Network(network_file):
        """writes all data into the network model data file (.net)
        @param *network_file network model file name 
        @return Finished writing file if the process goes well"""

	
	out_file = open(network_file, 'w') #open _network data file
	
	print '\n Writing _network_ file: ', network_file
	print '\n'
	t = time.clock()

	out_file.write( "# File created with PyEditNetwork 1.0");
	out_file.write( "\n# Mafalda Sousa mafsousa@ibmc.up.pt");
	out_file.write( "\n# Paulo Castro Aguiar pauloaguiar@fc.up.pt\n#");	

	# Recall template structure in case modification is needed
	out_file.write( "\n# The template for the structure of .net files is available at ~/NeuralSyns/doc/DotNet_Template.pdf.\n");

	out_file.write( "\nSTART\n")
	
	list=['\nMODELS:\t', str(total_models)]
	out_file.write(''.join(list))
	list1 = temp_list.keys()
	for i in range(len(models)):
	    list=['\n\t',models[i].model_label,'\t',str(models[i].n_statevars)]
	    out_file.write(''.join(list) )
	    for var in range(models[i].n_statevars):
		list=['\t', models[i].statevar_label[var]]
	        out_file.write(''.join(list))
	    list=['\t',str(models[i].n_params)]
	    out_file.write(''.join(list) )
	    for par in range(models[i].n_params):
		list=['\t', models[i].param_label[par]]
		out_file.write(''.join(list))
	
	
	out_file.write( "\n")	
	out_file.write('\nPOPULATIONS: ')

	#Check_data_type(total_populations,'int',inspect.stack()[0][1:3])
	out_file.write(str( total_populations))
  
	out_file.write( "\n\tlabel [char*10]:")
	for pop in range (total_populations):
		#Check_data_type(population[pop].label,'str',inspect.stack()[0][1:3])
		out_file.write( '\t')
		out_file.write(population[pop].label)

	out_file.write( "\n\tsize [#]:\t")
	for pop in range (total_populations):
		#Check_data_type(population[pop].n_neurons,'int',inspect.stack()[0][1:3])
		out_file.write( '\t')
		out_file.write( str(population[pop].n_neurons))


	out_file.write( "\n\tfdomains [#]:\t");
	for pop in range (total_populations):
		#Check_data_type(population[pop].n_fdomains,'int',inspect.stack()[0][1:3])
		out_file.write( '\t')
		out_file.write( str(population[pop].n_fdomains))

	out_file.write( "\n ");
	out_file.write( "\nFDOMAINS:");

	for pop in range (total_populations):
		out_file.write( '\n\t')
		out_file.write( population[pop].label);
		for fdm in range (population[pop].n_fdomains):
			#Check_data_type(population[pop].fdomain[fdm].fdm_ID,'int',inspect.stack()[0][1:3])
			#Check_data_type(population[pop].fdomain[fdm].label,'str',inspect.stack()[0][1:3])
			#Check_data_type(population[pop].fdomain[fdm].parent_fdomain,'int',inspect.stack()[0][1:3])
			#Check_data_type(population[pop].fdomain[fdm].model.model_label,'str',inspect.stack()[0][1:3])
			topology='('+ str(population[pop].fdomain[fdm].position.x) + ',' + str(population[pop].fdomain[fdm].position.y)+ ',' + str(population[pop].fdomain[fdm].position.z)+ ',' + str(population[pop].fdomain[fdm].position.r+')')
			list= ['\t[ ',str(population[pop].fdomain[fdm].fdm_ID),'\t',
					population[pop].fdomain[fdm].label,'\t',
					str(population[pop].fdomain[fdm].parent_fdomain),'\t',
					topology, '\t',
					population[pop].fdomain[fdm].model.model_label, ' ]']
			out_file.write( ''.join(list)) 
			
	
	out_file.write( "\n ")
	out_file.write( "\nHOMOGENEOUS SYNAPSES:\t")
	#Check_data_type(homogenity,'str',inspect.stack()[0][1:3])
	out_file.write( homogenity);
	out_file.write( "\n ")
	

	out_file.write("\nCONNECTION PAIRS: ")
	#Check_data_type(total_connection_pairs,'int',inspect.stack()[0][1:3])
	out_file.write(str(total_connection_pairs))
	n=-1
	for popi in range(total_populations):
	  for popj in range(total_populations):
	      if (connection[popi][popj]!=0):

		n=n+1
		out_file.write("\n\t PAIR:")
		#Check_data_type(connection[popi][popj].fdomain,'int',inspect.stack()[0][1:3])
		list=['\t',str(n),'\t[ ',population[popi].label,
					'\t',population[popj].label,' ]',
					'\t',population[popj].fdomain[connection[popi][popj].fdomain].label,
					'\t','[ ',str(popi),
					'\t',str(popj),
					' ]','\t',str(connection[popi][popj].fdomain)]
		out_file.write(''.join(list))
		
		#Check_data_type(connection[popi][popj].current_model.model_label,'str',inspect.stack()[0][1:3])
		
		list=['\n\t\t',connection[popi][popj].current_model.model_label]
		out_file.write(''.join(list))
		#Check_data_type(connection[popi][popj].plasticity_model.n_params,'int',inspect.stack()[0][1:3])
		if homogenity=='YES':
			for par in range(connection[popi][popj].current_model.n_params):
				#Check_data_type(connection[popi][popj].current_param[par],'float',inspect.stack()[0][1:3])
				list=['\t',str(connection[popi][popj].current_param[par])]
				out_file.write(''.join(list))
				
		
		list=['\n\t\t', connection[popi][popj].plasticity_model.model_label]
		out_file.write(''.join(list))
		#Check_data_type(connection[popi][popj].plasticity_model.n_params,'int',inspect.stack()[0][1:3])
		if homogenity=='YES':
			for par in range (connection[popi][popj].plasticity_model.n_params):
				#Check_data_type(connection[popi][popj].plasticity_param[par],'float',inspect.stack()[0][1:3])
				list=['\t',str(connection[popi][popj].plasticity_param[par])]
				out_file.write(''.join(list))

	      
	out_file.write( "\n");
	out_file.write( "\nNEURONS: ")
	#Check_data_type(total_neurons,'int',inspect.stack()[0][1:3])
	out_file.write( str(total_neurons))
	out_file.write( "\nSYNAPSES: ")
	#Check_data_type(total_synapses,'int',inspect.stack()[0][1:3])
	out_file.write( str(total_synapses))
   	out_file.write( "\n");
	prog = progressBar(0, total_neurons, 77)

	#print to file the information for each neuron
	for nrn in range(total_neurons): 

		if (nrn%10==0):
		    prog.updateAmount(nrn)
		    print prog, "\r",

		list=['\nNEURON ', str(nrn)]
		out_file.write( ''.join(list))
		#Check_data_type(neuron[nrn].position.x,'float',inspect.stack()[0][1:3])
		#Check_data_type(neuron[nrn].position.y,'float',inspect.stack()[0][1:3])
		#Check_data_type(neuron[nrn].position.z,'float',inspect.stack()[0][1:3])
		list=['\n\t(',str(neuron[nrn].position.x),', ',str(neuron[nrn].position.y),', ',str(neuron[nrn].position.z),')']
		out_file.write( ''.join(list))
		#repeat for all fdomains
		for fdm in range (neuron[nrn].n_fdomains):
			#Check_data_type(neuron[nrn].fdomain[fdm].Ra,'float',inspect.stack()[0][1:3])
			#Check_data_type(neuron[nrn].fdomain[fdm].V,'float',inspect.stack()[0][1:3])
			list=['\n\t',  str(neuron[nrn].fdomain[fdm].Ra), '\t', str(neuron[nrn].fdomain[fdm].V)]
			out_file.write( ''.join(list))
			for var in range( len(neuron[nrn].fdomain[fdm].model_statevar)):
				#Check_data_type(neuron[nrn].fdomain[fdm].model_statevar[var],'float',inspect.stack()[0][1:3])
				out_file.write( '\t')
				out_file.write(str(neuron[nrn].fdomain[fdm].model_statevar[var]))
			for par in range( len(neuron[nrn].fdomain[fdm].model_param)):
				#Check_data_type(neuron[nrn].fdomain[fdm].model_param[par],'float',inspect.stack()[0][1:3])
				out_file.write( '\t')
				out_file.write(str(neuron[nrn].fdomain[fdm].model_param[par]))
	
		out_file.write( '\n\tSPIKES:\t')
		#Check_data_type(neuron[nrn].n_spike_times,'int',inspect.stack()[0][1:3])
		out_file.write(	str(neuron[nrn].n_spike_times))
		for spk in range(neuron[nrn].n_spike_times):
		  #Check_data_type(neuron[nrn].spike_time[spk],'float',inspect.stack()[0][1:3])
		  list=['\t', str(neuron[nrn].spike_time[spk])]
		  out_file.write(''.join(list))
		out_file.write( "\n\tSYNAPSES:\t")
		#Check_data_type(neuron[nrn].n_synapses,'int',inspect.stack()[0][1:3])
		out_file.write( str(neuron[nrn].n_synapses))

		#repeat for all synapses

		for syn in range(neuron[nrn].n_synapses):

			#Check_data_type(neuron[nrn].synapse[syn].target_neuron,'int',inspect.stack()[0][1:3])
			#Check_data_type(neuron[nrn].synapse[syn].weight,'float',inspect.stack()[0][1:3])
			#Check_data_type(neuron[nrn].synapse[syn].delay,'float',inspect.stack()[0][1:3])
			list=['\n\t\t@ ',str(neuron[nrn].synapse[syn].target_neuron),'\t',str(neuron[nrn].synapse[syn].weight),'\t',str(neuron[nrn].synapse[syn].delay)]
			out_file.write( ''.join(list))
			#Check_data_type(neuron[nrn].synapse[syn].spikes_cue,'int',inspect.stack()[0][1:3])
			#Check_data_type(neuron[nrn].synapse[syn].activated,'int',inspect.stack()[0][1:3])
			#Check_data_type(neuron[nrn].synapse[syn].psr_time,'float',inspect.stack()[0][1:3])
			list=[ '\n\t\t\t',str(neuron[nrn].synapse[syn].spikes_cue),'\t',str(neuron[nrn].synapse[syn].activated),'\t',str(neuron[nrn].synapse[syn].psr_time)]
		  	out_file.write( ''.join(list))

			#PAULO
			pop_target = neuron[ neuron[nrn].synapse[syn].target_neuron ].population

			list=['\n\t\t\t', connection[neuron[nrn].population][pop_target].current_model.model_label]
			out_file.write( ''.join(list))
			for var in range(len(neuron[nrn].synapse[syn].current_statevar)):
				list=['\t',str(neuron[nrn].synapse[syn].current_statevar[var])]
				out_file.write( ''.join(list))
			if homogenity=='NO':
				for par in range (len(neuron[nrn].synapse[syn].current_param)):
					list=['\t',str(neuron[nrn].synapse[syn].current_param[par])]
					out_file.write( ''.join(list))
			list=['\n\t\t\t',connection[neuron[nrn].population][pop_target].plasticity_model.model_label]
			out_file.write( ''.join(list))
			for var in range (len(neuron[nrn].synapse[syn].plasticity_statevar)):
				list=['\t',str(neuron[nrn].synapse[syn].plasticity_statevar[var])]
				out_file.write( ''.join(list))
			if homogenity=='NO':
				for par in range(len(neuron[nrn].synapse[syn].plasticity_param)):
					list=['\t', str(neuron[nrn].synapse[syn].plasticity_param[par])]
					out_file.write( ''.join(list))
			out_file.write( "\n");
		if (neuron[nrn].n_synapses==0) :
		    out_file.write( "\n");

	out_file.write( "\nEND\n")

	print "\n\nFinished writing file. Elapsed time = ", time.clock()-t, 'secs.'
	out_file.close()



###############################################################################################################


	    
def Edit_Axon_Delay(pop_pre,pop_pos, velocity):
      """This function edit the synaptic axon delays, in function of it's velocity, from one source population to a population target
      @param pop_pre population source ID
      @param pop_pos population target ID
      @param velocity spike velocity in the connection in um/ms 
      @return new synaptic delays for each connection in ms"""	
      
      for nrn in range(population[pop_pre].n_neurons):
	  n=nrn+population[pop_pre].first_neuron
	  for syn in range(neuron[n].n_synapses):
	      nrn_i = n;
	      nrn_o = neuron[n].synapse[syn].target_neuron;
	      if (neuron[nrn_o].population==pop_pos):
		  d = sqrt( ( neuron[nrn_i].position.x - neuron[nrn_o].position.x )**(2) + ( neuron[nrn_i].position.y - neuron[nrn_o].position.y )**(2) + ( neuron[nrn_i].position.z - neuron[nrn_o].position.z )**(2) )
		  neuron[nrn_i].synapse[syn].delay = float(d / velocity)
	      


###############################################################################################################


      
def Produce_Synaptic_Gaussian_Profile(pop_source,pop_target,wfactor,sigma):
  	"""Produce a synaptic gaussian profile, by changing the weight of the synaptic connection from each neuron of the population source, using a weight factor and a standard deviation for the gaussian profile.
            @param pop_source popupulation source
 	    @param pop_target popupulation target
	    @param wfactor weight factor
	    @param sigma standard deviation of Gaussian distribution
	    @return new weight values for the synaptic connections"""	
  	for nrn in range (population[pop_source].first_neuron,population[pop_source].first_neuron + population[pop_source].n_neurons):
   	   for syn in range (neuron[nrn].n_synapses):
	 	 target_neuron=neuron[nrn].synapse[syn].target_neuron
	 	 if (neuron[target_neuron].population == pop_target) :
	    	  	#calculate distance
	    	 	dx=neuron[nrn].position.x-neuron[target_neuron].position.x
	    	 	dy=neuron[nrn].position.y-neuron[target_neuron].position.y
	     	 	dz=neuron[nrn].position.z-neuron[target_neuron].position.z
	    	 	dist_square = dx**2 + dy**2 + dz**2
	     	 	neuron[nrn].synapse[syn].weight = neuron[nrn].synapse[syn].weight * wfactor * exp(-dist_square/(2.0*sigma*sigma))



###############################################################################################################



def Define_Interception_Volume_Topology(nrn, pop_target, shape, *args):
	"""This function defines an interception volume topology (cubic, ellipsoid, cone or cylinder) for a given neuron ID and a target populations
	For each shape the argument *args represents:\n
	cubic		-> args=[a, b, c] # [length, weight, high]\n
	ellipsoid 	-> args=[a, b, c] # [radius in axis x, radius in axis y, radius in axis z]\n
	cone		-> args=[r, h]	  # [base radius, high]\n
	cylinder	-> args=[r, h]    # [base radius, high]
	@param nrn source neuron ID
	@param pop_target population target ID
	@param shape type of volume used as topology
	@param *args vector of volume arguments
	@return new targets position of neuron  nrn"""

	if shape == "cubic" or shape=="ellipsoid":
	      a=args[0]
	      b=args[1]
	      c=args[2]
	elif shape=="cylinder" or shape=="cone":
	      r=args[0]
	      h=args[1]
	      
	#bounding box pop_target	
	target_min_x = target_max_x = neuron[population[pop_target].first_neuron].position.x
	target_min_y = target_max_y = neuron[population[pop_target].first_neuron].position.y
	target_min_z = target_max_z = neuron[population[pop_target].first_neuron].position.z
	for n in range(population[pop_target].first_neuron,population[pop_target].first_neuron+population[pop_target].n_neurons):
	    x=neuron[n].position.x
	    y=neuron[n].position.y
	    z=neuron[n].position.z
	    if (x > target_max_x) :      target_max_x = x
	    if (x < target_min_x) :      target_min_x = x
	    if (y > target_max_y) :      target_max_y = y
	    if (y < target_min_y) :      target_min_y = y
	    if (z > target_max_z) :      target_max_z = z
	    if (z < target_min_z) :      target_min_z = z
	
	#bounding box pop_source
	pop_source=neuron[nrn].population
	source_min_x = source_max_x = neuron[population[pop_source].first_neuron].position.x
	source_min_y = source_max_y = neuron[population[pop_source].first_neuron].position.y
	source_min_z = source_max_z = neuron[population[pop_source].first_neuron].position.z
	for n in range(population[pop_source].first_neuron,population[pop_source].first_neuron+population[pop_source].n_neurons):
	    x=neuron[n].position.x
	    y=neuron[n].position.y
	    z=neuron[n].position.z
	    if (x > source_max_x) :      source_max_x = x
	    if (x < source_min_x) :      source_min_x = x
	    if (y > source_max_y) :      source_max_y = y
	    if (y < source_min_y) :      source_min_y = y
	    if (z > source_max_z) :      source_max_z = z
	    if (z < source_min_z) :      source_min_z = z    
	
	# source neuron position
	x=neuron[nrn].position.x
	y=neuron[nrn].position.y
	z=neuron[nrn].position.z
	
	# pop_target center position
	if (source_max_x-source_min_x<1): #this means there's only one neuron in the source population
	    x0 = 0.5*(target_max_x-target_min_x)+target_min_x
	    y0 = 0.5*(target_max_y-target_min_y)+target_min_y
	    z0 = 0.5*(target_max_z-target_min_z)+target_min_z
	else :
	    x0 = (x - source_min_x)*(target_max_x-target_min_x)/(source_max_x-source_min_x)+target_min_x
	    y0 = (y - source_min_y)*(target_max_y-target_min_y)/(source_max_z-source_min_y)+target_min_y
	    z0 = (z - source_min_z)*(target_max_z-target_min_z)/(source_max_z-source_min_z)+target_min_z
	
	# 
	targets=[]
	k=-1
	for n in range(population[pop_target].first_neuron,population[pop_target].first_neuron+population[pop_target].n_neurons):
	      x_nrn=neuron[n].position.x
	      y_nrn=neuron[n].position.y
	      z_nrn=neuron[n].position.z
	
     
	      if shape == "cubic" :
		  if (x0-a/2 < x_nrn < x0+a/2 and y0-b/2 < y_nrn < y0 + b/2 and z0-c/2 < z_nrn < z0+c/2) :
			targets.append(n)
	      elif shape == "ellipsoid":
		  if (((x_nrn-x0)/a)**2 + ((y_nrn-y0)/b)**2 + ((z_nrn-z0)/c)**2)  < 1.0 :
			targets.append(n)
	      elif shape == "cylinder":
		  if (((x_nrn-x0)**2 + (y_nrn-y0)**2 ) < r**2) and (z0-h/2 < z_nrn < z0+h/2) :
			targets.append(n)
	      elif shape == "cone":
		  if ((x_nrn-x0)**2 + (y_nrn-y0)**2 ) < (r*(z_nrn-z0)/h)**2 and z0 < z_nrn < z0+h :
			targets.append(n)
	if targets!=[] :
	    for syn in range(neuron[nrn].n_synapses):
		rnd=random.sample(targets,1)
		neuron[nrn].synapse[syn].target_neuron=rnd[0]
	else :	
	    print '\n No target neurons'
	    


###############################################################################################################



def Plot_RF(nrn,pop_target):
      """This function plots the connection volume topology of one neuron to one target population
	@param nrn neuron source ID
	@param pop_target population target ID
	@return the receptive field of neuron nrn 
      """

      fig = plt.figure()
      canvas = FigureCanvas(fig)
      #ax = fig.add_subplot(111, projection='3d')
      ax = Axes3D(fig) #PAULO
     
      x0=[0]*population[pop_target].n_neurons
      y0=[0]*population[pop_target].n_neurons
      z0=[0]*population[pop_target].n_neurons
      for n in range(population[pop_target].n_neurons):
	  x0[n]=neuron[n].position.x
	  y0[n]=neuron[n].position.y
	  z0[n]=neuron[n].position.z
	  
      ax.scatter(x0,y0,z0,c='b',marker='o',linewidths=0.1, alpha=0.2)
     
      x1=[0]*(neuron[nrn].n_synapses+1)
      y1=[0]*(neuron[nrn].n_synapses+1)
      z1=[0]*(neuron[nrn].n_synapses+1)
      x1[0]=neuron[nrn].position.x;
      y1[0]=neuron[nrn].position.y;
      z1[0]=neuron[nrn].position.z;
      for syn in range(neuron[nrn].n_synapses):
	  target=neuron[nrn].synapse[syn].target_neuron
	  if (neuron[target].population==pop_target):
	      x1[syn+1]=neuron[target].position.x
	      y1[syn+1]=neuron[target].position.y
	      z1[syn+1]=neuron[target].position.z

      ax.scatter(x1,y1,z1,c='r',marker='o',linewidths=1)
      # Set the title.
      ax.set_title("Connection volume topology",fontsize=12)
      ax.set_xlabel("x",fontsize=12)
      ax.set_ylabel("y",fontsize=12)
      ax.set_zlabel("z", fontsize=12)
      plt.show()
      # Save the generated Scatter Plot to a PNG file.

      canvas.print_figure("volume_topology.png",dpi=600)



###############################################################################################################



# All ready! Start the engine!
if __name__ == '__main__':
    main()
