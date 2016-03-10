clear;
load('data_512.mat');
x = x';

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

border = melf_border();

filter_gain = zeros(20, 256);
energy_melband = zeros(1, 20);

for bank_num = 1:20
	x_length = border(bank_num+2) - border(bank_num);
	x_length_inc = border(bank_num+1) - border(bank_num);
	x_length_dec = border(bank_num+2) - border(bank_num+1);

	for index = 0:x_length_inc-1
		filter_gain(bank_num, index+1) = index / x_length_inc;
	end

	for index = 0:x_length_dec
		filter_gain(bank_num, index+x_length_inc+1) = 1 - index / x_length_dec;
	end

	for index = 1:x_length+1
		energy_melband(bank_num) = energy_melband(bank_num) + power(border(bank_num)+index) * filter_gain(bank_num, index);
	end
end
