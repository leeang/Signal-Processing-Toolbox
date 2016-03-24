function cepstrum_coefficient = discrete_cosine_transform(energy_melband)
	global BANK_NUM;
	global FEAT_NUM;
	global dct_coef;

	cepstrum_coefficient = zeros(1, FEAT_NUM);
	for n = 0:FEAT_NUM-1
		sum = 0;
		for k = 0:BANK_NUM/2-1
			sum = sum + (energy_melband(k+1) + (-1)^n * energy_melband(20-k)) * dct_coef(n+1, k+1);
		end
		cepstrum_coefficient(n+1) = sqrt(2/BANK_NUM) * sum;
	end
end
