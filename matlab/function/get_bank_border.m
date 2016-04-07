function bank_border = get_bank_border()
	min_f = 1;
	max_f = 8000;

	min_mf = f_to_melf(min_f);
	max_mf = f_to_melf(max_f);

	mf_point = linspace(min_mf, max_mf, 22);

	bank_border = melf_to_f(mf_point);
end
