clear;
load('data_512.mat');
x = x';

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

border = get_bank_border();

filter_gain = zeros(20, 256);
energy_melband = zeros(1, 20);

for bank_num = 1:20
	x_length_inc = border(bank_num+1) - border(bank_num);
	x_length_dec = border(bank_num+2) - border(bank_num+1);

	% index = 1;
	% while (index-1)*16000/512 < border(bank_num)
	% 	index = index+1;
	% end
	index = ceil(border(bank_num)/16000*512) + 1;

	while (index-1)*16000/512 <= border(bank_num+1)
		filter_gain(bank_num, index) = ( (index-1)*16000/512-border(bank_num) ) / x_length_inc;
		index = index+1;
	end

	while (index-1)*16000/512 <= border(bank_num+2)
		filter_gain(bank_num, index) = 1 - ( (index-1)*16000/512 - border(bank_num+1) ) / x_length_dec;
		index = index+1;
	end
end

for bank_num = 1:20
	for index = 1:256
		energy_melband(bank_num) = energy_melband(bank_num) + power(index) * filter_gain(bank_num, index);
	end
end
