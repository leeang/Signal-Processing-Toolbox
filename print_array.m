function print_array(data, variable_name, permission, variable_type)
	N = length(data);

	if nargin == 1
		variable_name = 'data';
		permission = 'w';
		variable_type = '%f';
	end
	if nargin == 2
		permission = 'w';
		variable_type = '%f';
	end
	if nargin == 3
		variable_type = '%f';
	end

	filename = 'array.c';
	fid = fopen(filename, permission);

	fprintf(fid, 'float %s[%d] = {', variable_name, N);

	prefix = '';
	for n=1:N
		fprintf(fid, '%s', prefix);
		if (mod(n-1, 5)==0)
			fprintf(fid, '\n\t');
		end
		fprintf(fid, variable_type, data(n));
		prefix = ', ';
	end

	fprintf(fid, '\n};\n');
end
