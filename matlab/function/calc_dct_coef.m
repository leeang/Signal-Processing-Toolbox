function calc_dct_coef()
	global BANK_NUM;
	global FEAT_NUM;
	global dct_coef;

	dct_coef = zeros(FEAT_NUM, BANK_NUM);
	for n = 0:FEAT_NUM-1
		for k = 0:BANK_NUM-1
			dct_coef(n+1, k+1) = cos(pi * (k+0.5) * n / BANK_NUM);
		end
	end
end
