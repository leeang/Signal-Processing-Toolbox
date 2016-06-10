clear;

N = 16;

b_rect = ones(1, N);
b_hamming = hamming(N);
b_hanning = hanning(N);
b_blackman = blackman(N);

[h_rect, w_rect] = freqz(b_rect, 1, 2^15);
[h_hamming, w_hamming] = freqz(b_hamming, 1, 2^15);
[h_hanning, w_hanning] = freqz(b_hanning, 1, 2^15);
[h_blackman, w_blackman] = freqz(b_blackman, 1, 2^15);

h_rect = 20 * log10(abs(h_rect));
h_rect = h_rect - max(h_rect);
w_rect = w_rect / pi;

h_hamming = 20 * log10(abs(h_hamming));
h_hamming = h_hamming - max(h_hamming);
w_hamming = w_hamming / pi;

h_hanning = 20 * log10(abs(h_hanning));
h_hanning = h_hanning - max(h_hanning);
w_hanning = w_hanning / pi;

h_blackman = 20 * log10(abs(h_blackman));
h_blackman = h_blackman - max(h_blackman);
w_blackman = w_blackman / pi;

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

plot(0:N-1, b_rect);
hold on;
plot(0:N-1, b_hanning, '--');
plot(0:N-1, b_hamming, '-.');
plot(0:N-1, b_blackman, ':', 'LineWidth', 2);
grid on;
legend('rectangular', 'hannning', 'hamming', 'blackman')
title('windows in time domain ($N = 16$)', 'interpreter', 'latex');
xlabel('normalized frequency ($\times \pi$ rad/sample)', 'interpreter', 'latex');
ylabel('magnitude (linear scale)', 'interpreter', 'latex');
ylim([0 1.2]);

print('windows_time', '-dpdf');

fig = figure;
fig.Units = 'inches';
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

plot(w_rect, h_rect);
hold on;
plot(w_hanning, h_hanning, '--');
plot(w_hamming, h_hamming, '-.');
plot(w_blackman, h_blackman, ':', 'LineWidth', 1.5);
grid on;
legend('rectangular', 'hannning', 'hamming', 'blackman')
title('windows in frequency domain ($N = 16$)', 'interpreter', 'latex');
xlabel('normalized frequency ($\times \pi$ rad/sample)', 'interpreter', 'latex');
ylabel('magnitude (dB)', 'interpreter', 'latex');

print('windows_frequency', '-dpng', '-r300');
