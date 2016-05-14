ABSPATH = pwd;
addpath([ABSPATH '/../function']);

clear;

green_html = [0 128 0]/255;

WINDOW_LENGTH = 512;

global BANK_NUM;
BANK_NUM = 20;

border = get_bank_border();

frequency = cell(1, BANK_NUM);
amplitude = cell(1, BANK_NUM);

for bank_index = 1:BANK_NUM
	index = 1;
	for k = 1:WINDOW_LENGTH
		current = (k-1) * 16000 / WINDOW_LENGTH;
		if (current > border(bank_index) && current <= border(bank_index+2))
			frequency{bank_index}(index) = current;
			amplitude{bank_index}(index) = rand();

			if (current < border(bank_index+1))
				gain = (current - border(bank_index)) / (border(bank_index+1) - border(bank_index));
				bank_gain{bank_index}(index) = gain;
				bank_gain_matrix(bank_index, k) = gain;
			else
				gain = 1 - (current - border(bank_index+1)) / (border(bank_index+2) - border(bank_index+1));
				bank_gain{bank_index}(index) = gain;
				bank_gain_matrix(bank_index, k) = gain;
			end

			index = index + 1;
		end
	end
end

bank_index = 9;

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

subplot(2, 1, 1);
plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
hold on;
plot(frequency{bank_index}, bank_gain{bank_index}, '*');
stem(frequency{bank_index}, amplitude{bank_index}, ':');
title(['power spectrum overlaid with filter bank ' num2str(bank_index)], 'interpreter', 'latex');
xlabel('frequency (Hz)', 'interpreter', 'latex');
legend('gain (continuous)', 'gain (discrete)', 'power data points');

subplot(2, 1, 2);
plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
hold on;
stem(frequency{bank_index}, amplitude{bank_index} .* bank_gain{bank_index}, 'x');
title('filtered power spectrum', 'interpreter', 'latex');
xlabel('frequency (Hz)', 'interpreter', 'latex');
legend('gain', 'filtered power');
print('bank_filter_demostration', '-dpdf');

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';
imagesc(bank_gain_matrix);
color_vector = transpose(linspace(1, 0, 50));
colormap([color_vector color_vector color_vector]);
colorbar;
title('Mel filter bank gain', 'interpreter', 'latex');
xlabel('power $\hat{S}(k) \quad (k = 1, 2, \dots, 257)$', 'interpreter', 'latex');
ylabel('bank $m = 1, 2, \dots, 20$', 'interpreter', 'latex');
ax = gca;
ax.XTick = linspace(0, 256, 9);
grid on;

annotation('doublearrow', [.12 .525], [.21 .21], 'LineStyle', '--', 'Color', 'r');
annotation('doublearrow', [.525 .65], [.21 .21], 'Color', green_html);
print('mel_filter_bank_gain', '-dpdf');
