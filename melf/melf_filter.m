clear;
load('data_512.mat');

spectrum = fft(x);
magnitude = abs(spectrum);
power = magnitude .^ 2;

melf_border = melf_border();

after_mel_filter = zeros(256,22);

for n = 1:256
    for i = 2:21
        mel_filter_gain = 0;
        if n >= melf_border(i-1) && n <= melf_border(i);
            mel_filter_gain = (n - melf_border(i-1)) / (melf_border(i)- melf_border(i-1));
        elseif n > melf_border(i) && n <= melf_border(i+1);
            mel_filter_gain = (melf_border(i+1) - n) / (melf_border(i+1) - melf_border(i));
        end
        after_mel_filter(n, i) = mel_filter_gain * power(n);
    end
end
