F = 12;
M = 20;
dct_coef = zeros(F, M);

for n = 0:F-1
	for k = 0:M-1
		dct_coef(n+1, k+1) = cos(pi * (k+0.5) * n / M);
	end
end

cepstrum_coefficient = zeros(1, F);
for n = 0:F-1
	sum = 0;
	for k = 0:M-1
		sum = sum + energy_melband(k+1) * dct_coef(n+1, k+1);
	end

	cepstrum_coefficient(n+1) = sqrt(2/M) * sum;
end
