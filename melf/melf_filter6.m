clear;
load('data_512.mat');
x = x';

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

border = get_bank_border();

filter_gain_size = floor(border(22)/125) + floor(border(21)/125) - floor(border(2)/125) - floor(border(1)/125);
filter_gain = zeros(1, filter_gain_size);
energy_melband = zeros(1, 20);

filter_gain_index = 1;

for bank_num = 1:20
	x_length_inc = border(bank_num+1) - border(bank_num);
	x_length_dec = border(bank_num+2) - border(bank_num+1);

	offset = floor(border(bank_num)/125);

	index = 1;
	while (index+offset)*125 <= border(bank_num+1)
		filter_gain(filter_gain_index) = ( (index+offset)*125-border(bank_num) ) / x_length_inc;
		index = index+1;
		filter_gain_index = filter_gain_index + 1;
	end

	while (index+offset)*125 <= border(bank_num+2)
		filter_gain(filter_gain_index) = 1 - ( (index+offset)*125 - border(bank_num+1) ) / x_length_dec;
		index = index+1;
		filter_gain_index = filter_gain_index + 1;
	end
end

for bank_num = 1:20
	offset = floor(border(bank_num)/125);
	L = floor(border(bank_num+2)/125) - floor(border(bank_num)/125);

	if bank_num == 1
		filter_gain_offset = floor(border(1)/125);
	elseif bank_num ==2
		filter_gain_offset = floor(border(3)/125) - floor(border(1)/125);
	else
		filter_gain_offset = floor(border(bank_num+1)/125) + floor(border(bank_num)/125) - floor(border(2)/125) - floor(border(1)/125);
	end

	for index = 1:L
		energy_melband(bank_num) = energy_melband(bank_num) + power(index+offset+1) * filter_gain(index+filter_gain_offset);
	end
end

energy_melband = log10(energy_melband);
