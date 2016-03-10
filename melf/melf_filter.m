clear;
load('data_512.mat');
x = x';

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

border = melf_border();

filter_gain = zeros(20, 256);
energy_melband = zeros(1, 20);

for filter_num = 2:21
	x_length = border(filter_num+1) - border(filter_num-1);
	x_length_inc = border(filter_num) - border(filter_num-1);
	x_length_dec = border(filter_num+1) - border(filter_num);

	for index = 0:x_length_inc-1
		filter_gain(filter_num-1, index+1) = index / x_length_inc;
	end

	for index = 0:x_length_dec
		filter_gain(filter_num-1, index+x_length_inc+1) = 1 - index / x_length_dec;
	end

	for index = 1:x_length+1
		energy_melband(filter_num-1) = energy_melband(filter_num-1) + power(border(filter_num-1)+index) * filter_gain(filter_num-1, index);
	end
end
