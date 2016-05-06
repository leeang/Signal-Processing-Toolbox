clear;
load('../data/21');

x = data((1:1024)+19000);

y(1:512) = x(1:512) .* hamming(512);
y(513:1024) = x(513:1024) .* hamming(512);

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(2, 1, 1);
plot(x);
grid on;
xlim([0 1024]);
ylim([-0.6 0.6]);
ax = gca;
ax.XTick = linspace(0, 1024, 9);
title('2048 data points of word \textit{zero}', 'interpreter', 'latex');
xlabel('data point index', 'interpreter', 'latex');
ylabel('amplitude', 'interpreter', 'latex');

subplot(2, 1, 2);
plot(y);
grid on;
xlim([0 1024]);
ylim([-0.6 0.6]);
ax = gca;
ax.XTick = linspace(0, 1024, 9);
title('after hamming window', 'interpreter', 'latex');
xlabel('data point index', 'interpreter', 'latex');
ylabel('amplitude', 'interpreter', 'latex');
annotation('ellipse', [0.46 0.22 0.12 0.12], 'LineStyle' ,'-.', 'color', 'red');
