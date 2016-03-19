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

frame_data = zeros(WINDOW_LENGTH, 1);
voiced_cepstrum = zeros(180, 12);
voiced_cepstrum_index = 1;

frame_offset = 0;

for frame_num = 1:FRAME_NUM
	for index = 1:WINDOW_LENGTH
		frame_data(index) = after_preemphasis(index+frame_offset);
	end

	frame_data = hamming(WINDOW_LENGTH) .* frame_data;
	energy = sum(frame_data .^ 2);
	zc_num = zc_count(frame_data);

	if energy > 0.1
		voiced_cepstrum(voiced_cepstrum_index, :) = calc_mfcc(frame_data);
		voiced_cepstrum_index = voiced_cepstrum_index + 1;
	end

	frame_offset = frame_offset + WINDOW_LENGTH/2;
end