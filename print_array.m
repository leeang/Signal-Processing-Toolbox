function print_array(data)
	N = length(data);

	filename = 'array.c';
	fid = fopen(filename, 'w');

	fprintf(fid, 'float data[%d] = {', N);

	prefix = '';
	for n=1:N
		fprintf(fid, '%s', prefix);
		if (mod(n-1, 5)==0)
			fprintf(fid, '\n\t');
		end
		fprintf(fid, '%f', data(n));
		prefix = ', ';
	end

	fprintf(fid, '\n};\n');
end
