clear;
load('data_512.mat');
x = x';

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

bank_border = get_bank_border();

filter_gain = zeros(20, 256);
energy_melband = zeros(1, 20);

for bank_num = 1:20
	x_length = bank_border(bank_num+2) - bank_border(bank_num);
	x_length_inc = bank_border(bank_num+1) - bank_border(bank_num);
	x_length_dec = bank_border(bank_num+2) - bank_border(bank_num+1);

	for index = 0:x_length_inc-1
		filter_gain(bank_num, index+1) = index / x_length_inc;
	end

	for index = 0:x_length_dec
		filter_gain(bank_num, index+x_length_inc+1) = 1 - index / x_length_dec;
	end
end

% Remove zero rows
filter_gain( all(~filter_gain, 2), : ) = [];
% Remove zero columns
filter_gain( :, all(~filter_gain, 1) ) = [];

for bank_num = 1:20
	for index = 2:x_length
		energy_melband(bank_num) = energy_melband(bank_num) + power(bank_border(bank_num)+index) * filter_gain(bank_num, index-1);
	end
end
