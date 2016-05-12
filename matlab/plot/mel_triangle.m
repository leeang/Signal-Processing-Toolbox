ABSPATH = pwd;
addpath([ABSPATH '/../function']);

clear;

global BANK_NUM;
BANK_NUM = 20;

border = get_bank_border();

fig = figure;
fig.Units = 'inches';
fig.Position(3) = fig.Position(3) * 4 / 3;
pos = fig.Position;
fig.PaperSize = [pos(3) pos(4)];
fig.PaperPositionMode = 'Auto';

for bank_index = 1:BANK_NUM
	if bank_index <= 7
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0]);
	elseif bank_index <= 14
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0], ':', 'LineWidth', 2);
	else
		plot([border(bank_index) border(bank_index+1) border(bank_index+2)], [0 1 0], '-.');
	end
	hold on;
end

title('Mel filter banks', 'interpreter', 'latex');
xlabel('frequency (Hz)', 'interpreter', 'latex');
ylabel('gain (linear scale)', 'interpreter', 'latex');
legend(...
	'bank 1', 'bank 2', 'bank 3', 'bank 4', 'bank 5', 'bank 6', 'bank 7', 'bank 8', 'bank 9', 'bank 10',...
	'bank 11', 'bank 12', 'bank 13', 'bank 14', 'bank 15', 'bank 16', 'bank 17', 'bank 18', 'bank 19', 'bank 20',...
	'Location', 'eastoutside'...
);
print('mel_triangle', '-dpdf');
