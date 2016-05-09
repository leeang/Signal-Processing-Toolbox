function energy_melband = mel_filter(power_spectrum)
	global BANK_NUM;
	global bank_gain;
	global bank_gain_index_offset;
	global fft_index_offset;
	global bank_length;

	energy_melband = zeros(1, BANK_NUM);
	for bank_index = 1:BANK_NUM
		for index = 1:bank_length(bank_index)
			energy_melband(bank_index) = energy_melband(bank_index) + power_spectrum(index + fft_index_offset(bank_index)) * bank_gain(index + bank_gain_index_offset(bank_index));
		end
	end

	energy_melband = log10(energy_melband);
end
