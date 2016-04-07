ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

clear;

WINDOW_LENGTH = 512;
BANK_NUM = 20;

plot_bool = false;

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
			else
				bank_gain_matrix(bank_index, index) = 1 - (current-border(bank_index+1)) / (border(bank_index+2) - border(bank_index+1));
			end

			bank_gain(bank_gain_index) = bank_gain_matrix(bank_index, index);
			index = index+1;
			bank_gain_index = bank_gain_index+1;
		end
	end
	fft_index_first(bank_index) = min(fft_index(bank_index, :));
	fft_index_last(bank_index) = max(fft_index(bank_index, :));

	if plot_bool
		figure;
		hold on;
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
		stem(frequency(bank_index, :), amplitude(bank_index, :), ':');
		plot(frequency(bank_index, :), bank_gain_matrix(bank_index, :), 'x');
	end
end

save('mel_filter_coef', 'bank_gain', 'fft_index_first', 'fft_index_last', 'bank_gain_index_offset');
