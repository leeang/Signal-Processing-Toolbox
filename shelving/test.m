load('test_signal');
[shelf_b, shelf_a] = shelving(6, 1000, 16E3, 0.9, 'Treble_Shelf');
after_preemphasis = filter(shelf_b, shelf_a, x);
