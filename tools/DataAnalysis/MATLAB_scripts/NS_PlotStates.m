% This .m file loads a out_states file created by NeuralSyns with the
% flag -W <time interval> (or an electrode) and plots the profiles

% AUTHOR: Paulo de Castro Aguiar; paguiar@ibmc.up.pt; Apr 2011

function data = NS_PlotStates(filename)

% clear stuff
%clear all;
%close all;

% Import the file
newData = importdata(filename);

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData);
for i = 1:length(vars)
	assignin('base', vars{i}, newData.(vars{i}));
end

data = newData.data;

%clear('newData'); clear('vars'); clear('textdata'); clear('i');


n = size(data, 2) - 1;

figure, plot(data(:,1), data(:,2:n + 1));

title('State variable dynamics');
xlabel('time [ms]');
ylabel('values [unknown units]');

legend('Location', 'NorthEast')