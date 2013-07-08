% This .m file loads the out_spike_times.dat file created by NeuralSyns with 
% the flag -K and produces a raster plot

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


% draw the raster plot
figure
scatter( data(:,2), data(:,1),'.');
title('Spikes Raster Plot');
xlabel('time [ms]');
ylabel('neuron id [1]');
axis([0 data(end,2) 0 max(data(:,1)) ] );
grid;

% draw time histogram
figure
hist( data(:,2), 200);
title('Spike Times Histogram');
xlabel('time [ms]');
ylabel('counts');
grid;
