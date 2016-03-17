zerocrossing_sum = 0;
for n = 1:511
	z = x(n) * x(n+1);
	if z < 0 
		outcome = 1;
	else
		outcome = 0;
	end
	zerocrossing_sum = zerocrossing_sum + outcome;
end
