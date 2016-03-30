function data = read_mp3(index)
	[data, fs] = audioread(['mp3/google/uk-' num2str(index) '.mp3']);

	if fs ~= 16000
		error(['Sampling frequency: ' num2str(fs)  'Hz']);
	end

	l = length(data);
	n = (fs*3 - l)/2;

	data = [zeros(n, 1); data; zeros(n, 1)];
end
