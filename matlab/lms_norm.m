load('lms_data.mat');

LMS_LENGTH = 300;
STEP_SIZE = 0.04;

weights = zeros(1, LMS_LENGTH);

err = zeros(1, length(cmd_noise));

norm_factor = 0;

for n = 1:LMS_LENGTH
	output = 0;
	for k = 1:n
		output = output + weights(k) * noise(n-k+1);
	end

	err(n) = cmd_noise(n) - output;

	norm_factor = norm_factor + noise(n)^2;
	new_step_size = STEP_SIZE / (1+norm_factor);

	for k = 1:n
		weights(k) = weights(k) + new_step_size * err(n) * noise(n-k+1);
	end
end

for n = (LMS_LENGTH+1):length(cmd_noise)
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
end

figure(1);
subplot(3, 1, 1);
plot(noise);
ylim([-0.1 0.1]);

subplot(3, 1, 2);
plot(cmd_noise);
ylim([-0.1 0.1]);

subplot(3, 1, 3);
plot(err);
ylim([-0.1 0.1]);
