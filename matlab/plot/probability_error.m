clear;

MATLAB = [-748.558524942301 -693.591502676118 -794.413999311685 -791.078896434326 -807.180198767299 -820.267720641463 -729.856606906899 -881.475606812735 -732.690408195949 -824.839418112333 -755.016950339232 -797.975941257253 -784.797584825466 -792.878531789262 -895.462984087828 -828.797701182304 -809.271975503981 -908.269920249795 -758.972105417584 -855.478531793776 -589.097923434755 -831.625627497508 -969.644047720905 -753.803513342183 -822.526347905483 -779.075395777941 -1115.52768152416];
DSP = [-748.564200 -693.595150 -794.417950 -791.080150 -807.182550 -820.270200 -729.859800 -881.480300 -732.693800 -824.843700 -755.019800 -797.981300 -784.800150 -792.881000 -895.467200 -828.803050 -809.274500 -908.275100 -758.975050 -855.480800 -589.101850 -831.627700 -969.648400 -753.807200 -822.531900 -779.078850 -1115.544750];

percent_error = (DSP-MATLAB) ./ MATLAB;

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(2, 1, 1);
plot(MATLAB);
title('probabilities of 27 words computed by MATLAB', 'interpreter', 'latex');
xlabel('word 1 to word 27', 'interpreter', 'latex');
ylabel('probabilities', 'interpreter', 'latex');
grid on;
ax = gca;
ax.XTick = linspace(0, 27, 10);
xlim([0 28]);

subplot(2, 1, 2);
plot(DSP);
title('probabilities of 27 words computed by DSP', 'interpreter', 'latex');
xlabel('word 1 to word 27', 'interpreter', 'latex');
ylabel('probabilities', 'interpreter', 'latex');
grid on;
ax = gca;
ax.XTick = linspace(0, 27, 10);
xlim([0 28]);

print('probability_comparison', '-dpdf');

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

plot(percent_error);
title('relative error between probabilities computed by MATLAB and DSP', 'interpreter', 'latex');
xlabel('word 1 to word 27', 'interpreter', 'latex');
ylabel('$\frac{DSP - MATLAB}{MATLAB}$', 'interpreter', 'latex');
grid on;
ax = gca;
ax.XTick = linspace(0, 27, 10);
xlim([0 28]);

print('probability_error', '-dpdf');

return;

for word_index = 1:27
	fprintf('%f\t', MATLAB(word_index));
end
fprintf('\n');

for word_index = 1:27
	fprintf('%f\t', DSP(word_index));
end
fprintf('\n');
