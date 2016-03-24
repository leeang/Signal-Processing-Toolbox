clear;

load 'Model_27_1';

FEAT_NUM = 12;

data = Model{1};

mu = zeros(5, FEAT_NUM);
for index = 1:5
    mu(index, :) = data.Mu{index};
end
print_matrix(mu, 'mu', '%e');

inv_Var = zeros(5, FEAT_NUM);
det_Var = zeros(1, 5);
for index = 1:5
    var = data.Var{index} .* eye(FEAT_NUM, FEAT_NUM);
    inv_Var(index, :) = diag(inv(var));
    det_Var(index) = det(var);
end
print_matrix(inv_Var, 'inv_Var', '%e', 'a');

det_part = 1 ./ sqrt(det_Var * (2 * pi)^FEAT_NUM);
print_array(det_part, 'det_part', '%e', 'a');
