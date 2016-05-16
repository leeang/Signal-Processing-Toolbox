function print_matrix(data, variable_name, variable_type, permission)
	[M, N] = size(data);

	if nargin == 1
		variable_name = 'data';
		variable_type = '%f';
		permission = 'w';
	end
	if nargin == 2
		variable_type = '%f';
		permission = 'w';
	end
	if nargin == 3
		permission = 'w';
	end

	filename = 'data.c';
	fid = fopen(filename, permission);

	fprintf(fid, 'float %s[%d][%d] = {', variable_name, M, N);

	prefix1 = '';
	for m=1:M
		fprintf(fid, '%s\n', prefix1);
		fprintf(fid, '\t{');
		prefix2 = '';
		for n=1:N
			fprintf(fid, '%s', prefix2);
			fprintf(fid, variable_type, data(m, n));
			prefix2 = ', ';
		end
		fprintf(fid, '}');
		prefix1 = ',';
	end

	fprintf(fid, '\n};\n');
	fclose(fid);
end
