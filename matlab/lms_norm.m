ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

load('lms_data.mat');

LMS_LENGTH = 128;
STEP_SIZE = 0.1;
TOTAL_LENGTH = 46336;
UPDATE_INTERVAL = 4;

weights = zeros(1, LMS_LENGTH);

err = zeros(TOTAL_LENGTH, 1);

norm_factor = 0;

tic;

for n = 1:LMS_LENGTH
	output = 0;
	for k = 1:n
		output = output + weights(k) * noise(n-k+1);
	end

	err(n) = cmd_noise(n) - output;

	if mod(n, UPDATE_INTERVAL) == 1
	norm_factor = norm_factor + noise(n)^2;
	new_step_size = STEP_SIZE / (1+norm_factor);

	for k = 1:n
		weights(k) = weights(k) + new_step_size * err(n) * noise(n-k+1);
	end
	end
end

for n = (LMS_LENGTH+1):TOTAL_LENGTH
	output = 0;
	for k = 1:LMS_LENGTH
		output = output + weights(k) * noise(n-k+1);
	end

	err(n) = cmd_noise(n) - output;

	if mod(n, UPDATE_INTERVAL) == 1
	norm_factor = norm_factor - noise(n-LMS_LENGTH)^2 + noise(n)^2;
	new_step_size = STEP_SIZE / (1+norm_factor);

	for k = 1:LMS_LENGTH
		weights(k) = weights(k) + new_step_size * err(n) * noise(n-k+1);
	end
	end
end

toc;

plot_lim = [-max(cmd_noise) max(cmd_noise)];

figure;
subplot(3, 1, 1);
plot(noise);
ylim(plot_lim);

subplot(3, 1, 2);
plot(cmd_noise);
ylim(plot_lim);

subplot(3, 1, 3);
plot(err);
ylim(plot_lim);

% print_array(noise, 'noise', '%e');
% print_array(cmd_noise, 'cmd_noise', '%e', 'a');

% percent_error = (err_c-err) ./ err * 100;
% figure;
% plot(percent_error);
% title('$\frac{C - MATLAB}{MATLAB} \times 100\%$', 'interpreter', 'latex');
% xlabel('data points');
% ylabel('percent error (%)');
% grid on;
