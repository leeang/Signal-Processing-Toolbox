function emis_matrix = calc_emis_matrix(mfcc_matrix, word_index)
	global STATE_NUM;
	global FEAT_NUM;
	global Model;

	Var = Model{word_index}.Var;
	Mu = Model{word_index}.Mu;

	inv_Var = cell(STATE_NUM, 1);
	det_Var = cell(STATE_NUM, 1);

	for state = 1:STATE_NUM
		Var{state} = Var{state} .* eye(FEAT_NUM);

		inv_Var{state} = inv(Var{state});
		det_Var{state} = det(Var{state});
	end

	[obs_length, ~] = size(mfcc_matrix);

	emis_matrix = zeros(obs_length, STATE_NUM);

	for obs_index = 1:obs_length
		for state = 1:STATE_NUM 
			%--- Single Multivariate Gaussian ---%
			obs_minus_mean = mfcc_matrix(obs_index, :) - Mu{state};

			exp_part = -0.5 * obs_minus_mean * inv_Var{state} * transpose(obs_minus_mean);

			det_part = 1 / ( sqrt( (2*pi)^FEAT_NUM * det_Var{state} ) );
			det_part = log(det_part);

			emis_matrix(obs_index, state) = exp_part + det_part;
		end
	end
end
