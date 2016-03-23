clear;

load 'Model_27_1';

MFCC_NUM = 12;
STATE_NUM = 5;
WORD_NUM = 27;

mu = cell(1, WORD_NUM);
inv_Var = cell(1, WORD_NUM);
det_part = zeros(WORD_NUM, STATE_NUM);

for model_num = 1:WORD_NUM
	data = Model{model_num};

	mu_matrix = zeros(STATE_NUM, MFCC_NUM);
	for index = 1:STATE_NUM
		mu_matrix(index, :) = data.Mu{index};
	end
	mu{model_num} = mu_matrix;

	inv_Var_matrix = zeros(STATE_NUM, MFCC_NUM);
	det_Var = zeros(1, STATE_NUM);
	for index = 1:STATE_NUM
		var = data.Var{index} .* eye(MFCC_NUM, MFCC_NUM);
		inv_Var_matrix(index, :) = diag(inv(var));
		det_Var(index) = det(var);
	end
	inv_Var{model_num} = inv_Var_matrix;

	det_part_array = 1 ./ sqrt(det_Var * (2 * pi)^MFCC_NUM);
	det_part(model_num, :) = det_part_array;
end

print_3D(mu, 'mu', '%e');
print_3D(inv_Var, 'inv_Var', '%e', 'a');
print_matrix(det_part, 'det_part', '%e', 'a');
