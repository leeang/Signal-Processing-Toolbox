function P_star = viterbi(mfcc_matrix, word_index)
	global STATE_NUM;
	global Model;

	[obs_length, ~] = size(mfcc_matrix);

	log_trans = log(Model{word_index}.Trans);

	emis_matrix = calc_emis_matrix(mfcc_matrix, word_index);

	delta = zeros(obs_length, STATE_NUM);
	delta(1, 1) = emis_matrix(1, 1);

	for obs_index = 2:obs_length
		for j = 1:STATE_NUM
			delta_plus_trans = zeros(1, STATE_NUM);

			for i = 1:STATE_NUM
				delta_plus_trans(i) = delta(obs_index-1, i) + log_trans(i, j);
			end

			max_delta_plus_trans = max(delta_plus_trans);

			delta(obs_index, j) = max_delta_plus_trans + emis_matrix(obs_index, j);
		end
	end

	P_star = max(delta(obs_length, :));
end
