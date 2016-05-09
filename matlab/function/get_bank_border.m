function bank_border = get_bank_border()
	global BANK_NUM;

	min_f = 1;
	max_f = 8000;

	min_mf = f_to_melf(min_f);
	max_mf = f_to_melf(max_f);

	mf_point = linspace(min_mf, max_mf, BANK_NUM+2);

	bank_border = melf_to_f(mf_point);
	bank_border(1) = min_f;
	bank_border(BANK_NUM+2) = max_f;
end
