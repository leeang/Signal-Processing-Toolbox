clear;

MATLAB = [];
DSP = [];

percent_error = (DSP-MATLAB) ./ MATLAB;

fig = figure;
fig.Units = 'inches';
fig.Position(3) = fig.Position(3) * 4 / 3;
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

plot(percent_error);
title('relative error between probabilities computed by MATLAB and DSP', 'interpreter', 'latex');
xlabel('word 1 to word 27', 'interpreter', 'latex');
ylabel('$\frac{DSP - MATLAB}{MATLAB}$', 'interpreter', 'latex');
grid on;

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
