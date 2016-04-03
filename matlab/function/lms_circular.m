function err = lms_circular(cmd_noise, noise)
	STEP_SIZE = 0.04;

	global LMS_LENGTH;
	global weights;
	global noise_buffer;
	global norm_factor;

	SAMPLE_LENGTH = length(cmd_noise);
	err = zeros(SAMPLE_LENGTH, 1);

	for n = 1:LMS_LENGTH
		output = 0;
		for k = 1:n
			output = output + weights(k) * noise(n-k+1);
		end
		for k = n+1:LMS_LENGTH
			output = output + weights(k) * noise_buffer(n-k+LMS_LENGTH+1);
		end

		err(n) = cmd_noise(n) - output;

		norm_factor = norm_factor - noise_buffer(n)^2 + noise(n)^2;
		new_step_size = STEP_SIZE / (1+norm_factor);

		for k = 1:n
			weights(k) = weights(k) + new_step_size * err(n) * noise(n-k+1);
		end
		for k = n+1:LMS_LENGTH
			weights(k) = weights(k) + new_step_size * err(n) * noise_buffer(n-k+LMS_LENGTH+1);
		end
	end

	for n = (LMS_LENGTH+1):SAMPLE_LENGTH
		output = 0;
		for k = 1:LMS_LENGTH
			output = output + weights(k) * noise(n-k+1);
		end

		err(n) = cmd_noise(n) - output;

		norm_factor = norm_factor - noise(n-LMS_LENGTH)^2 + noise(n)^2;
		new_step_size = STEP_SIZE / (1+norm_factor);

		for k = 1:LMS_LENGTH
			weights(k) = weights(k) + new_step_size * err(n) * noise(n-k+1);
		end

		for k = 1:LMS_LENGTH
			noise_buffer(k) = noise(SAMPLE_LENGTH-LMS_LENGTH+k);
		end
	end

	for n = 1:LMS_LENGTH
		noise_buffer(n) = noise(SAMPLE_LENGTH-LMS_LENGTH+n);
	end
end
