data1 = audioread('mp3/uk-1.mp3');
data2 = audioread('mp3/uk-2.mp3');
data3 = audioread('mp3/uk-3.mp3');
data4 = audioread('mp3/uk-4.mp3');
data5 = audioread('mp3/uk-5.mp3');

noise = [data1; data2; data3; data4; data5];
noise = noise(1:48000);

noise_shift = [zeros(100, 1); noise];
noise_shift = noise_shift(1:48000);
