ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

clear;

WINDOW_LENGTH = 512;
BANK_NUM = 20;

border = get_bank_border();

fft_index = cell(1, BANK_NUM);
bank_length = zeros(1, BANK_NUM);
bank_gain_index_offset = zeros(1, BANK_NUM);
fft_index_offset = zeros(1, BANK_NUM);

bank_gain_index = 1;
for bank_index = 1:BANK_NUM

	bank_gain_index_offset(bank_index) = bank_gain_index-1;

	index = 1;
	for k = 1:WINDOW_LENGTH
		current = (k-1) * 16000 / WINDOW_LENGTH;
		if (current > border(bank_index) && current < border(bank_index+2))
			fft_index{bank_index}(index) = k;

			if (current < border(bank_index+1))
				bank_gain(bank_gain_index) = (current - border(bank_index)) / (border(bank_index+1) - border(bank_index));
			else
				bank_gain(bank_gain_index) = 1 - (current - border(bank_index+1)) / (border(bank_index+2) - border(bank_index+1));
			end

			index = index+1;
			bank_gain_index = bank_gain_index+1;
		end
	end

	fft_index_offset(bank_index) = min(fft_index{bank_index}) - 1;
	% index in C language starts from 0
	bank_length(bank_index) = max(fft_index{bank_index}) - min(fft_index{bank_index}) + 1;
end

save('mel_filter_coef', 'bank_gain', 'fft_index_offset', 'bank_length', 'bank_gain_index_offset');

return;

print_array(bank_gain, 'bank_gain_float', '%e');
print_array(fft_index_offset, 'fft_index_offset', '%d', 'a');
print_array(bank_length, 'bank_length', '%d', 'a');
print_array(bank_gain_index_offset, 'bank_gain_index_offset', '%d', 'a');
