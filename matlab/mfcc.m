clear;

ABSPATH = pwd;
% identify current folder (absolute path)
addpath([ABSPATH '/function']);
% add function path

load('data');

FRAME_NUM = 180;

global WINDOW_LENGTH;
WINDOW_LENGTH = 512;

global BANK_NUM;
BANK_NUM = 20;

global MFCC_NUM;
MFCC_NUM = 12;

global border;
border = get_bank_border();

global bank_gain;
calc_bank_gain();

global dct_coef;
calc_dct_coef();

[shelf_b, shelf_a] = shelving(6, 1000, 16E3, 0.9, 'Treble_Shelf');

shelf_b = shelf_b/4;
shelf_a = shelf_a/4;

after_preemphasis = filter(shelf_b, shelf_a, x);

frame_offset = 0;

for frame_num = 1:FRAME_NUM
	for index = 1:WINDOW_LENGTH
		data = x(index+frame_offset);
	end
	frame_offset = frame_offset + WINDOW_LENGTH/2;
end

after_hamming = hamming(WINDOW_LENGTH) .* after_preemphasis;
energy_after_hamming = sum(after_hamming .^ 2);

zc_num = zc_count(after_hamming);

cepstrum_coefficient = calc_mfcc(after_hamming);
