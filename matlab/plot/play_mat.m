for word_input = 1:27
	load(['../data/' num2str(word_input) '.mat']);
	sound(data, 16E3);
	pause(2);
end
