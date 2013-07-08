% This .m file loads the activity_levels.dat file created by NeuralSyns with
% the -A flag and plots the populations' activity levels

% AUTHOR: Paulo de Castro Aguiar; paguiar@ibmc.up.pt; Apr 2011


% clear stuff
%clear all;
%close all;


% Import the file
newData = importdata('out_activity_levels.dat');

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData);
for i = 1:length(vars)
	assignin('base', vars{i}, newData.(vars{i}));
end

clear('newData'); clear('vars'); clear('textdata'); clear('i');


total_populations = size(data, 2) - 1;

% plot data
figure, plot(data(:,1), data(:,2:total_populations + 1))
title('Populations Activity Levels');
xlabel('time [ms]');
ylabel('activity level [%]');
grid;

legend('Location', 'NorthEast')