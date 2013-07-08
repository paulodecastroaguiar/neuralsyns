import math, random
D = 800.0
for nrn in range( population[1].first_neuron, population[1].first_neuron + population[1].n_neurons ) :
    y = 0.25 * neuron[nrn].position.y
    sign = y/abs(y)
    if abs(y) < D :
        x = y * y / D
    else :       
        x = D + 2.0 * ( abs(y) - D )
        y = sign * D
    neuron[nrn].position.x = x + 20.0 * (2.0 * random.random() - 1)
    neuron[nrn].position.y = y + 20.0 * (2.0 * random.random() - 1)

Write_Network('hippocampus_mod.net')
