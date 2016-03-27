function print_3D(data, variable_name, variable_type, permission)
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

	[~, CELL_NUM] = size(data);
	matrix = data{1};
	[M, N] = size(matrix);
	fprintf(fid, 'float %s[%d][%d][%d] = {\n\t{', variable_name, CELL_NUM, M, N);

	prefix1 = '';
	for cell_index = 1:CELL_NUM
		fprintf(fid, '%s', prefix1);
		matrix = data{cell_index};
		prefix2 = '';
		for m=1:M
			fprintf(fid, '%s\n\t\t{', prefix2);
			prefix3 = '';
			for n=1:N
				fprintf(fid, '%s', prefix3);
				fprintf(fid, variable_type, matrix(m, n));
				prefix3 = ', ';
			end
			fprintf(fid, '}');
			prefix2 = ',';
		end
		prefix1 = '} , {';
		fprintf(fid, '\n\t');
	end

	fprintf(fid, '}\n};\n');
end
