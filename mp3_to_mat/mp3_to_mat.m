clear;

folder = 'google-uk';

if exist(folder, 'dir') ~= 7
	mkdir(folder);
end

for word_index = 1:27
	data = read_mp3(word_index);
	filename = [folder '/' num2str(word_index) '.mat'];
	save(filename, 'data');
end
