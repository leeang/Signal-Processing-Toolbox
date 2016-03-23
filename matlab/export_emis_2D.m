clear;

load 'Model_27_1';

MFCC_NUM = 12;

data = Model{1};

mu = zeros(5, MFCC_NUM);
for index = 1:5
    mu(index, :) = data.Mu{index};
end
print_matrix(mu, 'mu', '%e');

inv_Var = zeros(5, MFCC_NUM);
det_Var = zeros(1, 5);
for index = 1:5
    var = data.Var{index} .* eye(MFCC_NUM, MFCC_NUM);
    inv_Var(index, :) = diag(inv(var));
    det_Var(index) = det(var);
end
print_matrix(inv_Var, 'inv_Var', '%e', 'a');

det_part = 1 ./ sqrt(det_Var * (2 * pi)^MFCC_NUM);
print_array(det_part, 'det_part', '%e', 'a');
