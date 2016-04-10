function calc_bank_gain()
	global BANK_NUM;
	global border;
	global bank_gain;

	bank_gain_size = floor(border(22)/125) + floor(border(21)/125) - floor(border(2)/125) - floor(border(1)/125);
	bank_gain = zeros(1, bank_gain_size);

	bank_gain_index = 1;

	for bank_num = 1:BANK_NUM
		x_length_inc = border(bank_num+1) - border(bank_num);
		x_length_dec = border(bank_num+2) - border(bank_num+1);

		offset = floor(border(bank_num)/125);

		index = 1;
		while (index+offset)*125 <= border(bank_num+1)
			bank_gain(bank_gain_index) = ( (index+offset)*125-border(bank_num) ) / x_length_inc;
			index = index+1;
			bank_gain_index = bank_gain_index + 1;
		end

		while (index+offset)*125 <= border(bank_num+2)
			bank_gain(bank_gain_index) = 1 - ( (index+offset)*125 - border(bank_num+1) ) / x_length_dec;
			index = index+1;
			bank_gain_index = bank_gain_index + 1;
		end
	end
end
