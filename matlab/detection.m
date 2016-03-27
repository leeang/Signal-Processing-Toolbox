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

global border;
border = get_bank_border();

global bank_gain;
calc_bank_gain();

global dct_coef;
calc_dct_coef();

global shelf_b;
global shelf_a;

[shelf_b, shelf_a] = shelving(6, 1000, 16E3, 0.9, 'Treble_Shelf');

shelf_b = shelf_b/4;
shelf_a = shelf_a/4;

load('data');

mfcc_matrix = calc_mfcc_matrix(x);

for word_index = 1:WORD_NUM
	probability = viterbi(mfcc_matrix, word_index);
	% Viterbi(Model{word_index}, mfcc_matrix);
end
