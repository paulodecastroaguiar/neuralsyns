% This .m file loads the out_states.dat file created by NeuralSyns with the
% flag -V <time interval> (or an electrode) and plots the V profiles

% AUTHOR: Paulo de Castro Aguiar; paguiar@ibmc.up.pt; Nov 2006


% clear stuff
%clear all;
%close all;

load out_states.dat;
total_neurons = size(out_states, 2) - 1;

figure, plot(out_states(:,1), out_states(:,2:total_neurons + 1));

title('Membrane potential dynamics');
xlabel('time [ms]');
ylabel('membrane potential [mV]');

legend('Location', 'NorthEast')