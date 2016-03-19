clear;

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

load('data_512.mat');

energy_melband = mel_filter(x);
cepstrum_coefficient = discrete_cosine_transform(energy_melband);
