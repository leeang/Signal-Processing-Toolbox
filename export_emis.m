clear;

load 'Model_27_1';

FEAT_NUM = 12;
STATE_NUM = 5;
WORD_NUM = 27;

mu = cell(1, WORD_NUM);
inv_Var = cell(1, WORD_NUM);
det_part = zeros(WORD_NUM, STATE_NUM);
trans = cell(1, WORD_NUM);

for model_num = 1:WORD_NUM
	data = Model{model_num};

	for state = 1:STATE_NUM
		mu{model_num}(state, :) = data.Mu{state};

		Var = data.Var{state} .* eye(FEAT_NUM);

		inv_Var{model_num}(state, :) = diag(inv(Var));

		det_Var = det(Var);
		det_Var = 1 / sqrt(det_Var * (2 * pi)^FEAT_NUM);
		det_part(model_num, state) = det_Var;
	end

	trans{model_num} = data.Trans;
end

print_3D(mu, 'mu', '%e');
print_3D(inv_Var, 'inv_Var', '%e', 'a');
print_3D(trans, 'trans', '%e', 'a');
print_matrix(det_part, 'det_part', '%e', 'a');
