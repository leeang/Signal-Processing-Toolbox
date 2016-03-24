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

	mu_matrix = zeros(STATE_NUM, FEAT_NUM);
	for index = 1:STATE_NUM
		mu_matrix(index, :) = data.Mu{index};
	end
	mu{model_num} = mu_matrix;

	inv_Var_matrix = zeros(STATE_NUM, FEAT_NUM);
	for index = 1:STATE_NUM
		Var = data.Var{index} .* eye(FEAT_NUM);
		inv_Var_matrix(index, :) = diag(inv(Var));

		det_Var = det(Var);
		det_Var = 1 / sqrt(det_Var * (2 * pi)^FEAT_NUM);
		det_part(model_num, index) = det_Var;
	end
	inv_Var{model_num} = inv_Var_matrix;

    trans{model_num} = data.Trans;
end

print_3D(mu, 'mu', '%e');
print_3D(inv_Var, 'inv_Var', '%e', 'a');
print_3D(trans, 'trans', '%e', 'a');
print_matrix(det_part, 'det_part', '%e', 'a');
