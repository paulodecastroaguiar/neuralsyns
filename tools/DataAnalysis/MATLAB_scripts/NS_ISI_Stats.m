% This .m file loads the data.dat file created by NeuralSyns with 
% the flag -K and calculates ISI means and stdevs

% AUTHOR: Paulo de Castro Aguiar; pauloaguiar@fc.up.pt; Apr 2011


% clear stuff
%clear all;
%close all;


% Import the file
newData = importdata('out_spike_times.dat');

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData);
for i = 1:length(vars)
	assignin('base', vars{i}, newData.(vars{i}));
end

clear('newData'); clear('vars'); clear('textdata'); clear('i');

total_spikes = size(data, 1);
total_neurons = max(data(:,1)) + 1;

ISIs = zeros(total_spikes - total_neurons, 2); %-total_neurons removes the first (inf) ISI

last_spike_time = -inf * ones(1, total_neurons);

% convert spike_times vector (list of times) to binary spike_trains vector
% and create an array with ISI's information
c = 1;
for spk = 1:total_spikes    
    nrn = data(spk,1);
	isi = data(spk,2) - last_spike_time(nrn+1);
	if( isi < Inf )
		ISIs(c,1) = nrn;
		ISIs(c,2) = isi;
		c = c + 1;
	end 
	last_spike_time(nrn+1) = data(spk,2);

end

interspike_interval_stats = zeros(2, total_neurons); %[mean; stdev]
for nrn = 1:total_neurons
	i = find( ISIs(:,1) == nrn - 1 );
    interspike_interval_stats(1, nrn) = mean( ISIs(i,2) );
    interspike_interval_stats(2, nrn) = std(  ISIs(i,2) );
end

figure
plot(interspike_interval_stats');
title('ISI mean and std');
xlabel('neuron id [1]');
ylabel('ISI [ms]');
legend('mean', 'std', 'Location', 'NorthEast')
grid

figure
hist( ISIs(:,2), 100 );
title('ISIs histogram');
xlabel('ISI [ms]');
ylabel('counts [1]');

figure
plot(1000./interspike_interval_stats(1,:));
title('Mean Firing Frequencies');
xlabel('neuron id [1]');
ylabel('Freq [Hz]');
grid

