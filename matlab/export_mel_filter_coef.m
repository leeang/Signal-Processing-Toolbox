ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

clear;

WINDOW_LENGTH = 512;
BANK_NUM = 20;

plot_bool = false;
export_bool = false;

border = get_bank_border();

bank_gain_index = 1;
for bank_index = 1:BANK_NUM

	bank_gain_index_offset(bank_index) = bank_gain_index-1;

	index = 1;
	for k = 1:WINDOW_LENGTH
		current = (k-1) * 16000 / WINDOW_LENGTH;
		if (current > border(bank_index) && current < border(bank_index+2))
			frequency(bank_index, index) = current;
			amplitude(bank_index, index) = rand();
			fft_index(bank_index, index) = k;

			if (current < border(bank_index+1))
				bank_gain_matrix(bank_index, index) = (current-border(bank_index)) / (border(bank_index+1) - border(bank_index));
				bank_gain_matrix_sparse(bank_index, k) = bank_gain_matrix(bank_index, index);
			else
				bank_gain_matrix(bank_index, index) = 1 - (current-border(bank_index+1)) / (border(bank_index+2) - border(bank_index+1));
				bank_gain_matrix_sparse(bank_index, k) = bank_gain_matrix(bank_index, index);
			end

			bank_gain(bank_gain_index) = bank_gain_matrix(bank_index, index);
			index = index+1;
			bank_gain_index = bank_gain_index+1;
		end
	end
	fft_index_offset(bank_index) = min(fft_index(bank_index, :)) - 1;
	fft_index_length(bank_index) = max(fft_index(bank_index, :)) - min(fft_index(bank_index, :)) + 1;

	if bank_index == 9
		fig = figure;
		fig.Units = 'inches';
		pos = fig.Position;
		fig.PaperSize = [pos(3) pos(4)];
		fig.PaperPositionMode = 'Auto';
		subplot(2, 1, 1);
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
		hold on;
		plot(frequency(bank_index, :), bank_gain_matrix(bank_index, :), 'x');
		stem(frequency(bank_index, :), amplitude(bank_index, :), ':');
		title(['power spectrum overlaid with filter bank ' num2str(bank_index)], 'interpreter', 'latex');
		xlabel('frequency (Hz)', 'interpreter', 'latex');
		legend('bank gain (continuous)', 'bank gain (discrete)', 'power spectrum data points');
		subplot(2, 1, 2);
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
		hold on;
		stem(frequency(bank_index, :), amplitude(bank_index, :) .* bank_gain_matrix(bank_index, :));
		title('filtered power spectrum', 'interpreter', 'latex');
		xlabel('frequency (Hz)', 'interpreter', 'latex');
		legend('bank gain', 'filtered power spectrum');
		if plot_bool
			print('mel_bank_9', '-dpdf');
		end
	end
end

save('mel_filter_coef', 'bank_gain', 'fft_index_offset', 'fft_index_length', 'bank_gain_index_offset');

fig = figure;
fig.Units = 'inches';
fig.Position(3) = fig.Position(3) * 4 / 3;
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';
imagesc(bank_gain_matrix_sparse);
vector = transpose(linspace(1, 0, 10));
colormap([vector vector vector]);
colorbar;
title('Mel filter gain', 'interpreter', 'latex');
xlabel('Data point index', 'interpreter', 'latex');
ylabel('Bank index', 'interpreter', 'latex');
ax = gca;
ax.XTick = linspace(0, 256, 9);
grid on;
if plot_bool
	print('mel_filter_sparse_matrix', '-dpdf');
end

if export_bool
	print_array(bank_gain, 'bank_gain', '%e');
	print_array(fft_index_offset, 'fft_index_offset', '%d', 'a');
	print_array(fft_index_length, 'fft_index_length', '%d', 'a');
	print_array(bank_gain_index_offset, 'bank_gain_index_offset', '%d', 'a');
end
