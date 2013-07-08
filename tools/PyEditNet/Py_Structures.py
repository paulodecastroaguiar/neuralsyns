# -*- coding: utf-8 -*-
"""
 @file Py_Structures.py
 @author Mafalda Sousa

All Network structures defined as class objects
"""

population = []
total_populations = 0
model = []
total_models = 0
homogeneity = 'YES'
connection = []
total_connections = 0
total_synapses = 0
neuron = []
total_neurons = 0


class MODEL_DYNAMICS(object):
    def __init__(self,model_label=None,n_params=None,param_label=None,n_statevars=None,statevar_label=None):
	self.model_label	= model_label 		
	self.n_params		= n_params		 
	self.param_label	= param_label		
	self.n_statevars	= n_statevars		
	self.statevar_label 	= statevar_label	
 	  
class FDOMAIN(object):
      def __init__(self, Ra=None, V=None, model_params=None, model_statevar=None):
          self.Ra		= Ra
	  self.V		= V
	  self.model_params 	= model_params
	  self.model_statevar 	= model_statevar

class POSITION(object):
      def __init__(self, x=None, y=None, z=None, r=None):
	self.x	= x
	self.y	= y
	self.z	= z
	self.r	= r
	
class NEURON(object):
    def __init__(self, position=None, synapse=None, n_synapses=None, population=None, fdomain=None, n_fdomains=None,spike_time=None, last_spike_time_index=None):
        self.position	= position
        self.synapse	= synapse
        self.n_synapses	= n_synapses
        self.population = population
        self.fdomain	= fdomain
        self.n_fdomains	= n_fdomains
        self.spike_time = spike_time
        self.n_spike_times = last_spike_time_index

class FDOMAIN_GLOBAL(object):
    def __init__(self, label=None, parent_fdomain=None, position=None, Ra=None, model=None):
             self.label		= label
             self.parent_fdomain= parent_fdomain
             self.position	= position
             self.model 	= model
    
class POPULATION(object):
    def __init__(self, label=None, n_neurons=None, first_neuron=None, n_fdomains=None, fdomain=None):
        self.label	=label
        self.n_neurons 	= n_neurons
        self.first_neuron = first_neuron
        self.n_fdomains	= n_fdomains
        self.fdomain 	= fdomain

class SYNAPSES(object):
    def __init__(self, weight=None, delay=None, target_neuron=None, spikes_cue=None, activated=None, psr_time=None, current_param=None, current_statevar=None, plasticity_param=None, plasticity_statevar=None):
	self.weight		= weight
	self.delay		= delay
	self.target_neuron	= target_neuron
	self.spikes_cue		= spikes_cue
	self.activated		= activated
	self.psr_time		= psr_time
	self.current_param 	= current_param
	self.current_statevar 	= current_statevar
	self.plasticity_param 	= plasticity_param
	self.plasticity_statevar= plasticity_statevar
        
class CONNECTION(object):
    def __init__(self, label=None, fdomain=None, current_model=None, current_param=None, plasticity_model=None, plasticity_param=None):
	self.label		= label
	self.fdomain		= fdomain
        self.current_model	= current_model
        self.current_param 	= current_param
        self.plasticity_model	= plasticity_model
        self.plasticity_param	= plasticity_param
