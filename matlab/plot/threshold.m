clear;

WINDOW_LENGTH = 512;

green_html = [0 128 0]/255;

ABSPATH = pwd;
addpath([ABSPATH '/../function']);

load('../data/21');

a_shelf = [1 -1.52379693166806 0.649345913666990];
b_shelf = [1.86185636138765 -3.10285167865561 1.36654429926689];

after_preemphasis = filter(b_shelf, a_shelf, data);

frame_data = zeros(WINDOW_LENGTH, 1);
frame_offset = 0;

for frame_num = 1:180
	for index = 1:WINDOW_LENGTH
		frame_data(index) = after_preemphasis(index+frame_offset);
	end

	frame_data = hamming(WINDOW_LENGTH) .* frame_data;
	energy(frame_num) = sum(frame_data .^ 2);
	zc(frame_num) = zc_count(frame_data);

	frame_offset = frame_offset + WINDOW_LENGTH/2;
end

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(3, 1, 1);
plot(linspace(0, 3, 48000), data);
ylim([-0.7 0.7]);
title('word \textit{zero} waveform', 'interpreter', 'latex');
xlabel('time (s)', 'interpreter', 'latex');
ylabel('magnitude', 'interpreter', 'latex');

subplot(3, 1, 2);
plot(energy);
title('frame energy', 'interpreter', 'latex');
xlabel('frame index', 'interpreter', 'latex');
ylabel('enery', 'interpreter', 'latex');

subplot(3, 1, 3);
plot(zc);
title('frame zero-crossing count', 'interpreter', 'latex');
xlabel('frame index', 'interpreter', 'latex');
ylabel('zero-crossing count', 'interpreter', 'latex');

print('threshold_example1', '-dpng', '-r300');

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(3, 1, 1);
plot(linspace(0, 3, 48000), data);
xlim([1 2.5]);
ylim([-0.7 0.7]);
title('word \textit{zero} waveform (zoomed in)', 'interpreter', 'latex');
xlabel('time (s)', 'interpreter', 'latex');
ylabel('magnitude', 'interpreter', 'latex');

subplot(3, 1, 2);
plot(energy);
xlim([60 150]);
title('frame energy (zoomed in)', 'interpreter', 'latex');
xlabel('frame index', 'interpreter', 'latex');
ylabel('enery', 'interpreter', 'latex');

subplot(3, 1, 3);
plot(zc);
xlim([60 150]);
title('frame zero-crossing count (zoomed in)', 'interpreter', 'latex');
xlabel('frame index', 'interpreter', 'latex');
ylabel('zero-crossing count', 'interpreter', 'latex');

annotation('rectangle', [0.19 0.125 0.06 0.79], 'LineStyle' ,'-.', 'color', green_html);
annotation('rectangle', [0.252 0.125 0.3 0.79], 'LineStyle' ,'--', 'color', 'red');

print('threshold_example2', '-dpng', '-r300');
