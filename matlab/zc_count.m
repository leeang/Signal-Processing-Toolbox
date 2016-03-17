function zc_num = zc_count(data)
	zc_num = 0;
	for n = 1:length(data)-1
		z = data(n) * data(n+1);
		if z < 0
			outcome = 1;
		else
			outcome = 0;
		end
		zc_num = zc_num + outcome;
	end
end
