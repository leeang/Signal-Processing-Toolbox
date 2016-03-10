function border = melf_border()
	WINDOW_SIZE = 512;

	min_f = 0;
	max_f = 8000;

	min_mf = f_to_melf(min_f);
	max_mf = f_to_melf(max_f);

	mf_point = linspace(min_mf, max_mf, 22);

	f_point = melf_to_f(mf_point);

	border = round(WINDOW_SIZE * f_point / 16E3);
end
