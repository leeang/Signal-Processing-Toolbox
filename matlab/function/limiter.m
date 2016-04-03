function output = limiter(arg)
	if arg > 1
		output = 1;
	else if arg < -1
		output = -1;
	else
		output = arg;
	end
end
