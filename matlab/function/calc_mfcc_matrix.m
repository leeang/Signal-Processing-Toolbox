function mfcc_matrix = calc_mfcc_matrix(x)
	global FRAME_NUM;
	global WINDOW_LENGTH;
	global BANK_NUM;
	global FEAT_NUM;

	global border;
	global bank_gain;
	global dct_coef;

	global shelf_b;
	global shelf_a;

	after_preemphasis = filter(shelf_b, shelf_a, x);

	frame_data = zeros(WINDOW_LENGTH, 1);
	obvs_length = 1;

	frame_offset = 0;

	for frame_num = 1:FRAME_NUM
		for index = 1:WINDOW_LENGTH
			frame_data(index) = after_preemphasis(index+frame_offset);
		end

		frame_data = hamming(WINDOW_LENGTH) .* frame_data;
		energy = sum(frame_data .^ 2);
		zc_num = zc_count(frame_data);

		if energy > 0.1
			mfcc_matrix(obvs_length, :) = calc_mfcc(frame_data);
			obvs_length = obvs_length + 1;
		end

		frame_offset = frame_offset + WINDOW_LENGTH/2;
	end
end
