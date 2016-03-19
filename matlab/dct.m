clear;

global BANK_NUM;
BANK_NUM = 20;

global MFCC_NUM;
MFCC_NUM = 12;

global border;
border = get_bank_border();

global bank_gain;
calc_bank_gain();

global dct_coef;
calc_dct_coef();

load('data_512.mat');

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

energy_melband = zeros(1, BANK_NUM);
for bank_num = 1:BANK_NUM
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
		energy_melband(bank_num) = energy_melband(bank_num) + power(index+offset+1) * bank_gain(index+filter_gain_offset);
	end
end

energy_melband = log10(energy_melband);

cepstrum_coefficient = zeros(1, F);
for n = 0:F-1
	sum = 0;
	for k = 0:BANK_NUM-1
		sum = sum + energy_melband(k+1) * dct_coef(n+1, k+1);
	end

	cepstrum_coefficient(n+1) = sqrt(2/BANK_NUM) * sum;
end
