clear;

load('data_512');
WINDOW_LENGTH = 512;

energy = sum(x .^ 2);

[shelf_b, shelf_a] = shelving(6, 1000, 16E3, 0.9, 'Treble_Shelf');

shelf_b = shelf_b/4;
shelf_a = shelf_a/4;

after_preemphasis = filter(shelf_b, shelf_a, x);
energy_after_preemphasis = sum(after_preemphasis .^ 2);

after_hamming = hamming(WINDOW_LENGTH) .* after_preemphasis;
energy_after_hamming = sum(after_hamming .^ 2);

zc_num = zc_count(x);
