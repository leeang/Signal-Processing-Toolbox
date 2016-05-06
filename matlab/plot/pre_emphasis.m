clear;

load('../data');

x = transpose(x);
N = length(x);
X = fft(x);
X = abs(X);

X = X(1:N/2);
frequency_vector = (0:N/2-1) / N * 16E3;

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';
stem(frequency_vector, X, 'marker', 'none');
grid on;
title('word \textit{zero} frequency spectrum', 'interpreter', 'latex');
xlabel('frequency (Hz)', 'interpreter', 'latex');
ylabel('magnitude (linear scale)', 'interpreter', 'latex');
print('zero_fft', '-dpng', '-r300');

a_shelf = [1 -1.52379693166806 0.649345913666990];
b_shelf = [1.86185636138765 -3.10285167865561 1.36654429926689];
[h_shelf, w_shelf] = freqz(b_shelf, a_shelf);

b_high_pass = [1 -0.95];
[h_high_pass, w_high_pass] = freqz(b_high_pass);

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';
plot(w_shelf / pi * 8000, 20*log10(abs(h_shelf)));
hold on;
plot(w_high_pass / pi * 8000, 20*log10(abs(h_high_pass)), '-.');
grid on;
title('pre-emphasis filter amplitude response', 'interpreter', 'latex');
xlabel('frequency (Hz)', 'interpreter', 'latex');
ylabel('magnitude (dB)', 'interpreter', 'latex');
legend('shelving', 'high-pass FIR', 'location', 'southeast');
print('pre_emphasis_filter', '-dpdf');
