function print_matrix(data)
	[M, N] = size(data);

	filename = 'matrix.c';
	fid = fopen(filename, 'w');

	fprintf(fid, 'float data[%d][%d] = {', M, N);

	prefix1 = '';
	for m=1:M
		fprintf(fid, '%s\n', prefix1);
		fprintf(fid, '\t{');
		prefix2 = '';
		for n=1:N
			fprintf(fid, '%s', prefix2);
			fprintf(fid, '%f', data(m, n));
			prefix2 = ', ';
		end
		fprintf(fid, '}');
		prefix1 = ',';
	end

	fprintf(fid, '\n};\n');
end
