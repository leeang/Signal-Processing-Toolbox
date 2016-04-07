clear;

ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

global Model;
load('Model.mat');

global FRAME_NUM;
FRAME_NUM = 180;

global WINDOW_LENGTH;
WINDOW_LENGTH = 512;

global BANK_NUM;
BANK_NUM = 20;

global FEAT_NUM;
FEAT_NUM = 12;

WORD_NUM = 27;

global STATE_NUM;
STATE_NUM = 5;

global ENERGY_THRESHOLD;
ENERGY_THRESHOLD = 0.04;

global dct_coef;
calc_dct_coef();

% Mel filter coefficients
global bank_gain;
global bank_gain_index_offset;
global fft_index_offset;
global fft_index_length;
load('mel_filter_coef');

global shelf_b;
global shelf_a;

[shelf_b, shelf_a] = shelving(6, 1000, 16E3, 0.9, 'Treble_Shelf');

shelf_b = shelf_b/4;
shelf_a = shelf_a/4;

load('data');

mfcc_matrix = calc_mfcc_matrix(x);

probability = zeros(1, WORD_NUM);
for word_index = 1:WORD_NUM
	probability(word_index) = viterbi(mfcc_matrix, word_index);
	% Viterbi(Model{word_index}, mfcc_matrix);
end

[probability_max, result] = max(probability);

% for word_index = 1:27
% 	fprintf('%f\t', probability(word_index))
% end
