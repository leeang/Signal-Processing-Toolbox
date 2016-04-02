clear;

ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

load('lms_data.mat');

global LMS_LENGTH;
LMS_LENGTH = 300;

global weights;
weights = zeros(1, LMS_LENGTH);

global norm_factor;
norm_factor = 0;

global noise_buffer;
noise_buffer = zeros(LMS_LENGTH, 1);

vector1 = 1:length(cmd_noise)/2;
vector2 = vector1 + length(cmd_noise)/2;

err1 = lms_circular(cmd_noise(vector1), noise(vector1));
err2 = lms_circular(cmd_noise(vector2), noise(vector2));
err_test = [err1; err2];
