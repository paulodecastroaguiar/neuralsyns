# -*- coding: utf-8 -*-

import generate_schemas.ml_schemas as ml
import generate_schemas._net as net
import generate_schemas._meta as meta
import generate_schemas._mml as mml
import generate_schemas._bio as bio
import generate_schemas._cml as cml

import pyxb.utils.domutils as domutils
import pyxb.binding.datatypes
from pyxb.namespace.builtin import XMLSchema_instance as xsi
import sys
import math
import time
import os.path
import Py_Edit_Network as py_net
import Py_Structures as Py_S



###############################################################################################################

def main():
	print len(sys.argv), str(sys.argv) 
	if len(sys.argv) < 3 :
		print "\nERROR: Name of .xml file missing.\nRe-run with the network file you want to load and the neuroML file name you want to export .\nExample:\n\npyNetBuilder.sh mynetworkfile.net neuroMLfile.xml\n\n"
		os._exit(0)
	else:
		network_file = sys.argv[1]
		neuroML_file = sys.argv[2]
		print network_file, neuroML_file
		Export_NeuroML(network_file, neuroML_file) 	#Read the network file and export to neuroML

		print "\nRead network.net files and export it to neuroML Level3 v1.8.1 format.\n"
	



###############################################################################################################

def Export_NeuroML (network_file, neuroML_file ):
    """Reads .net file and exports it to NeuroML (Level3) format
    @param *network_file (.net) network model file name
    @param *neuroML_file neuroML file name
    @return Finished reading file if the process goes well"""

    filename = network_file
    if os.path.isfile ( filename )==False:
        print('\nERROR: unable to read neurons _network_ data file')
        os._exit(0)
        return None

    bds = pyxb.utils.domutils.BindingDOMSupport()
    bds.declareNamespace(xsi)
    network = ml.neuroml()
    py_net.Read_Network(network_file)

    t = time.clock()

    network.properties  
    network.length_units='micrometer'
    network.populations = net.Populations()
    network.cells = ml.Level3Cells()
    xsi.createExpandedName('schemaLocation')
    network.channels = cml.ChannelML()
    network.channels.units = meta.Units( "Physiological Units")



    # Channels
    print('writing channels')
    for sy in range(1):
        network.channels.synapse_type.append(cml.SynapseType())

    sy = 0  # synapse_type Dual Exponential

    network.channels.synapse_type[sy].name = 'Exp2Syn'
    network.channels.synapse_type[sy].status= meta.Status()
    network.channels.synapse_type[sy].status.value_ = 'stable'
    network.channels.synapse_type[sy].doub_exp_syn = cml.DoubleExponentialSynapse()
    network.channels.synapse_type[sy].doub_exp_syn.max_conductance = bio.ConductanceValue()
    network.channels.synapse_type[sy].doub_exp_syn.reversal_potential = bio.VoltageValue()
    network.channels.synapse_type[sy].doub_exp_syn.decay_time = bio.TimeConstantValue(1.0)
    network.channels.synapse_type[sy].doub_exp_syn.rise_time = bio.TimeConstantValueIncZero()
    network.channels.synapse_type[sy].doub_exp_syn.max_conductance = 0.001
    network.channels.synapse_type[sy].doub_exp_syn.reversal_potential = 0
    network.channels.synapse_type[sy].doub_exp_syn.decay_time = 5.0
    network.channels.synapse_type[sy].doub_exp_syn.rise_time = 1.0

    for ch in range (2):
        network.channels.channel_type.append(cml.ChannelType())
        
    ch = 0 # channel_type Passive conductance

    network.channels.channel_type[ch].name = 'pas'
    network.channels.channel_type[ch].status= meta.Status()
    network.channels.channel_type[ch].status.value_ = 'stable'
    network.channels.channel_type[ch].current_voltage_relation = cml.CurrentVoltageRelation()
    network.channels.channel_type[ch].current_voltage_relation.cond_law = 'ohmic'
    network.channels.channel_type[ch].current_voltage_relation.ion = 'non_specific'
    network.channels.channel_type[ch].current_voltage_relation.default_erev = -70.0
    network.channels.channel_type[ch].current_voltage_relation.default_gmax = 0.001

    ch = 1 # channel-type Integrate and fire

    network.channels.channel_type[ch].name = 'IF'
    network.channels.channel_type[ch].status= meta.Status()
    network.channels.channel_type[ch].status.value_ = 'stable'
    network.channels.channel_type[ch].current_voltage_relation = cml.CurrentVoltageRelation()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire = cml.IntegrateAndFire()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.threshold = bio.VoltageValue()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.t_refrac = bio.TimeValue()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.v_reset = bio.VoltageValue()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.g_refrac = bio.ConductanceValue()
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.threshold = -50.0 
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.t_refrac = 5.0
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.v_reset = -70.0
    network.channels.channel_type[ch].current_voltage_relation.integrate_and_fire.g_refrac = 100.0

    for pop in range(py_net.total_populations):

        # Cells
        print('writing cells')
        network.cells.cell.append(ml.Level3Cell())
        network.cells.cell[pop].name = 'Cell_' + str(pop)
        network.cells.cell[pop].notes = meta.Notes()
        network.cells.cell[pop].notes = 'Cell type created in NeuralSyns simulation environment'
        network.cells.cell[pop].segments = mml.CTD_ANON_3()
        network.cells.cell[pop].cables = mml.CTD_ANON_4()
        network.cells.cell[pop].biophysics = ml.Level3Biophysics()
        network.cells.cell[pop].biophysics.units="Physiological Units"
        network.cells.cell[pop].connectivity = net.Level3Connectivity()
        
       
        for fdm in range (py_net.population[pop].n_fdomains):

            # Cables        
            
            network.cells.cell[pop].cables.cable.append(mml.Cable())
            network.cells.cell[pop].cables.cable[fdm].id = py_net.population[pop].fdomain[fdm].fdm_ID
            network.cells.cell[pop].cables.cable[fdm].name = str('group_' + py_net.population[pop].fdomain[fdm].label)
            network.cells.cell[pop].cables.cable[fdm].group = meta.Group()
            network.cells.cell[pop].cables.cable[fdm].group.append('group_' + py_net.population[pop].fdomain[fdm].label)
            
    for pop in range(py_net.total_populations):            
        for fdm in range (py_net.population[pop].n_fdomains):
            # Fdomains (Segments)

            network.cells.cell[pop].segments.segment.append(mml.Segment())
            network.cells.cell[pop].segments.segment[fdm].id = py_net.population[pop].fdomain[fdm].fdm_ID
            network.cells.cell[pop].segments.segment[fdm].name = py_net.population[pop].fdomain[fdm].label
            network.cells.cell[pop].segments.segment[fdm].proximal = meta.Point()
            network.cells.cell[pop].segments.segment[fdm].distal = meta.Point()
            if py_net.population[pop].fdomain[fdm].fdm_ID == 0:
                network.cells.cell[pop].segments.segment[fdm].proximal.x = py_net.population[pop].fdomain[fdm].position.x
                network.cells.cell[pop].segments.segment[fdm].proximal.y = py_net.population[pop].fdomain[fdm].position.y
                network.cells.cell[pop].segments.segment[fdm].proximal.z = py_net.population[pop].fdomain[fdm].position.z
                network.cells.cell[pop].segments.segment[fdm].proximal.diameter = float(py_net.population[pop].fdomain[fdm].position.r) * 2.0
                network.cells.cell[pop].segments.segment[fdm].distal.x = py_net.population[pop].fdomain[fdm].position.x
                network.cells.cell[pop].segments.segment[fdm].distal.y = py_net.population[pop].fdomain[fdm].position.y
                network.cells.cell[pop].segments.segment[fdm].distal.z = py_net.population[pop].fdomain[fdm].position.z
                network.cells.cell[pop].segments.segment[fdm].distal.diameter = float(py_net.population[pop].fdomain[fdm].position.r) * 2.0
            else :
                network.cells.cell[pop].segments.segment[fdm].parent = py_net.population[pop].fdomain[fdm].parent_fdomain
                network.cells.cell[pop].segments.segment[fdm].proximal.x = py_net.population[pop].fdomain[py_net.population[pop].fdomain[fdm].parent_fdomain].position.x
                network.cells.cell[pop].segments.segment[fdm].proximal.y = py_net.population[pop].fdomain[py_net.population[pop].fdomain[fdm].parent_fdomain].position.y
                network.cells.cell[pop].segments.segment[fdm].proximal.z = py_net.population[pop].fdomain[py_net.population[pop].fdomain[fdm].parent_fdomain].position.z
                network.cells.cell[pop].segments.segment[fdm].proximal.diameter = float(py_net.population[pop].fdomain[py_net.population[pop].fdomain[fdm].parent_fdomain].position.r) * 2    
                network.cells.cell[pop].segments.segment[fdm].distal.x = py_net.population[pop].fdomain[fdm].position.x
                network.cells.cell[pop].segments.segment[fdm].distal.y = py_net.population[pop].fdomain[fdm].position.y
                network.cells.cell[pop].segments.segment[fdm].distal.z = py_net.population[pop].fdomain[fdm].position.z
                network.cells.cell[pop].segments.segment[fdm].distal.diameter = float(py_net.population[pop].fdomain[fdm].position.r) * 2.0
            network.cells.cell[pop].segments.segment[fdm].cable = network.cells.cell[pop].cables.cable[fdm].id


        # Biophysics
        print('writing biophysics')
         

        for m in range(2):
            network.cells.cell[pop].biophysics.mechanism.append(bio.Mechanism())
            network.cells.cell[pop].biophysics.mechanism[m].type = bio.MechanismType('Channel Mechanism')

        m = 0 # IF model    
        network.cells.cell[pop].biophysics.mechanism[m].name = network.channels.channel_type[1].name
        network.cells.cell[pop].biophysics.mechanism[m].passive_conductance = 'true'
        network.cells.cell[pop].biophysics.mechanism[m].parameter.append(bio.NamedParameter())
        network.cells.cell[pop].biophysics.mechanism[m].parameter[0].group.append( network.cells.cell[pop].cables.cable[0].name )
        network.cells.cell[pop].biophysics.mechanism[m].parameter[0].name = 'V_rest'
        network.cells.cell[pop].biophysics.mechanism[m].parameter[0].value_ = -70.0
        network.cells.cell[pop].biophysics.mechanism[m].parameter.append(bio.NamedParameter())
        network.cells.cell[pop].biophysics.mechanism[m].parameter[1].group.append( network.cells.cell[pop].cables.cable[0].name )
        network.cells.cell[pop].biophysics.mechanism[m].parameter[1].name = 'gmax'
        network.cells.cell[pop].biophysics.mechanism[m].parameter[1].value_ = 0.3

        m = 1 # passive model
        network.cells.cell[pop].biophysics.mechanism[m].name = network.channels.channel_type[0].name
        network.cells.cell[pop].biophysics.mechanism[m].passive_conductance = 'true'
        network.cells.cell[pop].biophysics.mechanism[m].parameter.append(bio.NamedParameter())
        for fdm in range (0,py_net.population[pop].n_fdomains) :
            network.cells.cell[pop].biophysics.mechanism[m].parameter[0].group.append( network.cells.cell[pop].cables.cable[fdm].name )
        network.cells.cell[pop].biophysics.mechanism[m].parameter[0].name = 'e'
        network.cells.cell[pop].biophysics.mechanism[m].parameter[0].value_ = 0.001
        network.cells.cell[pop].biophysics.mechanism[m].parameter.append(bio.NamedParameter())
        for fdm in range (0,py_net.population[pop].n_fdomains) :
            network.cells.cell[pop].biophysics.mechanism[m].parameter[1].group.append( network.cells.cell[pop].cables.cable[fdm].name )
        network.cells.cell[pop].biophysics.mechanism[m].parameter[1].name = 'gmax'
        network.cells.cell[pop].biophysics.mechanism[m].parameter[1].value_ = 0.3

        # Capacitance and axial resistance        
        network.cells.cell[pop].biophysics.spec_capacitance = bio.SpecCapacitance()
        network.cells.cell[pop].biophysics.spec_capacitance.parameter.append(bio.UnnamedParameter())
        network.cells.cell[pop].biophysics.spec_capacitance.parameter[0].value_ = 1.0 # default_value
        network.cells.cell[pop].biophysics.spec_capacitance.parameter[0].group.append('all')
        network.cells.cell[pop].biophysics.spec_axial_resistance = bio.SpecAxialResistance()
        for fdm in range (py_net.population[pop].n_fdomains):
            network.cells.cell[pop].biophysics.spec_axial_resistance.parameter.append(bio.UnnamedParameter())
            network.cells.cell[pop].biophysics.spec_axial_resistance.parameter[fdm].value_ = 0.1 #default_value
            network.cells.cell[pop].biophysics.spec_axial_resistance.parameter[fdm].group.append( network.cells.cell[pop].cables.cable[fdm].name )
            
    #connections
    #default (connections between any segment will be available in neuroconstruct)
    for popi in range(py_net.total_populations):
        network.cells.cell[popi].connectivity.potential_syn_loc.append(net.PotentialSynLoc())
        network.cells.cell[popi].connectivity.potential_syn_loc[0].group_.append('all')
        network.cells.cell[popi].connectivity.potential_syn_loc[0].synapse_type = network.channels.synapse_type[0].name
                    
    for pop in range(py_net.total_populations): 
                
        # Populations    
        print('writing populations')
        network.populations.population.append(net.Population())
        network.populations.population[pop].name = py_net.population[pop].label
        network.populations.population[pop].cell_type_ = network.cells.cell[pop].name
        network.populations.population[pop].instances = net.Instances()
        network.populations.population[pop].instances.size = py_net.population[pop].n_neurons    
        for nrn in range (py_net.population[pop].n_neurons):
            network.populations.population[pop].instances.instance.append(net.CellInstance())
            network.populations.population[pop].instances.instance[nrn].id = nrn
            network.populations.population[pop].instances.instance[nrn].location =  meta.Point3D()
            network.populations.population[pop].instances.instance[nrn].location.x =  py_net.neuron[py_net.population[pop].first_neuron+nrn].position.x
            network.populations.population[pop].instances.instance[nrn].location.y =  py_net.neuron[py_net.population[pop].first_neuron+nrn].position.y
            network.populations.population[pop].instances.instance[nrn].location.z =  py_net.neuron[py_net.population[pop].first_neuron+nrn].position.z

              
    # Connectivity (projections and connections)
    print('writing synapses')
    if py_net.total_synapses != 0:

        network.projections = net.Projections()
        network.projections.units = meta.Units('Physiological Units')
        pairs = 0
        for popi in range (py_net.total_populations):
            #total_connections = 0
            #total_connections = py_net.population[popi].n_neurons * py_net.neuron[py_net.population[popi].first_neuron].n_synapses
            for popj in range (py_net.total_populations):
                if py_net.connection[popi][popj]!=0:
                    pop_source = popi
                    pop_target = popj
                    print(popi, popj)
                    network.projections.projection.append(net.Projection())
                    network.projections.projection[pairs].name = py_net.connection[pop_source][pop_target].label
                    network.projections.projection[pairs].source_ = py_net.population[pop_source].label
                    network.projections.projection[pairs].target_ = str(py_net.population[pop_target].label)
                    network.projections.projection[pairs].synapse_props.append( net.GlobalSynapticProperties())
                    network.projections.projection[pairs].synapse_props[0].synapse_type_ = network.channels.synapse_type[0].name
                    network.projections.projection[pairs].synapse_props[0].internal_delay = py_net.neuron[py_net.population[pop_source].first_neuron].synapse[0].delay
                    network.projections.projection[pairs].synapse_props[0].weight = py_net.neuron[py_net.population[pop_source].first_neuron].synapse[0].weight
                    network.projections.projection[pairs].connections = net.Connections()
                   
                        
                    
                    c = 0
                    post_cell = []
                    pre_cell = []
                    for nrn in range (py_net.population[pop_source].first_neuron,py_net.population[pop_source].first_neuron + py_net.population[pop_source].n_neurons):
                        for syn in range(py_net.neuron[nrn].n_synapses):
                            p_target = py_net.neuron[py_net.neuron[nrn].synapse[syn].target_neuron].population
                            post_cell_id = py_net.neuron[nrn].synapse[syn].target_neuron - py_net.population[p_target].first_neuron
                                                  
                            if (p_target == pop_target) :
                                post_cell.append(int(post_cell_id))
                                pre_cell.append(nrn)
                                c = c + 1
                   
                    network.projections.projection[pairs].connections.size = c
                    for conn in range (c):
                        network.projections.projection[pairs].connections.connection.append(net.Connection())
                        network.projections.projection[pairs].connections.connection[conn].properties_.append(net.LocalSynapticProperties())
                        network.projections.projection[pairs].connections.connection[conn].id = conn
                        network.projections.projection[pairs].connections.connection[conn].pre_cell_id = pre_cell[conn] - py_net.population[pop_source].first_neuron
                        network.projections.projection[pairs].connections.connection[conn].pre_segment_id = 0 # source segment is always soma
                        network.projections.projection[pairs].connections.connection[conn].post_cell_id = post_cell[conn]
                        network.projections.projection[pairs].connections.connection[conn].post_segment_id = py_net.connection[pop_source][pop_target].fdomain
                        network.projections.projection[pairs].connections.connection[conn].post_fraction_along = meta.ZeroToOne()
                        network.projections.projection[pairs].connections.connection[conn].post_fraction_along = 0.5
                        
                    pairs = pairs + 1

          
    #Inputs

    network.inputs = net.Inputs()
    network.inputs.units = meta.Units('SI Units')
    network.inputs.input.append(net.Input())
    network.inputs.input[0].name = 'RandomInput'
    network.inputs.input[0].target = net.InputTarget()
    network.inputs.input[0].target.population = network.populations.population[0].name
    network.inputs.input[0].target.sites = net.InputSites()
    network.inputs.input[0].target.sites.size = 1
    network.inputs.input[0].target.sites.site.append(net.InputSite())
    network.inputs.input[0].target.sites.site[0].cell_id = net.CellIdInNetwork()
    network.inputs.input[0].target.sites.site[0].cell_id = 0
    network.inputs.input[0].random_stim = net.RandomStim()
    network.inputs.input[0].random_stim.frequency = bio.FrequencyValue()
    network.inputs.input[0].random_stim.frequency = 10
    network.inputs.input[0].random_stim.synaptic_mechanism = network.channels.synapse_type[0].name

    print('Writing your xml network')

    file(neuroML_file, 'w').write(network.toxml("utf-8"))
    print "\n\nFinished writing file. Elapsed time = ", time.clock()-t, 'secs.'

###############################################################################################################



# All ready! Start the engine!
if __name__ == '__main__':
    main()

