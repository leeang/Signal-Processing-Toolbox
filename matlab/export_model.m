% export speech model into .c file

clear;

ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

load('Model.mat');

FEAT_NUM = 13;
STATE_NUM = 5;
WORD_NUM = 27;

mu = cell(1, WORD_NUM);
inv_Var = cell(1, WORD_NUM);
det_part = zeros(WORD_NUM, STATE_NUM);
trans = cell(1, WORD_NUM);

for word_num = 1:WORD_NUM
	data = Model{word_num};

	for state = 1:STATE_NUM
		mu{word_num}(state, :) = data.Mu{state};

		Var = data.Var{state} .* eye(FEAT_NUM);

		inv_Var{word_num}(state, :) = diag(inv(Var)) * -0.5;

		det_Var = det(Var);
		det_Var = 1 / sqrt(det_Var * (2 * pi)^FEAT_NUM);
		det_part(word_num, state) = log(det_Var);
	end

	trans{word_num} = log(data.Trans);
end

print_3D(mu, 'mu', '%e');
print_3D(inv_Var, 'inv_Var', '%e', 'a');
print_3D(trans, 'trans', '%e', 'a');
print_matrix(det_part, 'det_part', '%e', 'a');
