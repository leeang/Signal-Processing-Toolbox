clear;
load('data_512.mat');

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

border = melf_border();

after_mel_filter = zeros(256,22);

for n = 1:256
    for i = 2:21
        mel_filter_gain = 0;
        if n >= border(i-1) && n <= border(i);
            mel_filter_gain = (n - border(i-1)) / (border(i)- border(i-1));
        elseif n > border(i) && n <= border(i+1);
            mel_filter_gain = (border(i+1) - n) / (border(i+1) - border(i));
        end
        after_mel_filter(n, i) = mel_filter_gain * power(n);
    end
end
