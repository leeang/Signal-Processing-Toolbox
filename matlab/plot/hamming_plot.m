clear;
load('../data/21');

green_color = [0 128 0]/255;

x = data((1:1024)+19000);

y(1:512) = x(1:512) .* hamming(512);
y(513:1024) = x(513:1024) .* hamming(512);

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(2, 1, 1);
plot(1:512, hamming(512));
hold on;
plot(513:1024, hamming(512), '-.');
grid on;
xlim([0 1024]);
ax = gca;
ax.XTick = linspace(0, 1024, 9);
title('\texttt{hamming(512) without overlap}', 'interpreter', 'latex');
xlabel('data point index', 'interpreter', 'latex');
ylabel('gain', 'interpreter', 'latex');
legend('window 1', 'window 2');

subplot(2, 1, 2);
plot(1:512, hamming(512));
hold on;
plot((1:512)+256, hamming(512), '-.');
plot((1:512)+512, hamming(512), '--');
grid on;
xlim([0 1024]);
ax = gca;
ax.XTick = linspace(0, 1024, 9);
title('\texttt{hamming(512) with overlap 256}', 'interpreter', 'latex');
xlabel('data point index', 'interpreter', 'latex');
ylabel('gain', 'interpreter', 'latex');
legend('window 1', 'window 2', 'window 3');

annotation('rectangle', [0.46 0.112 0.12 0.336], 'LineStyle' ,'-.', 'color', green_color);

print('hamming_overlap', '-dpdf');

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

print('hamming_bell_shape', '-dpdf');
