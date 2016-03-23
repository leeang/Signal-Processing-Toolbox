#include <stdio.h>

void times2(int input[2][3], int output[2][3])
{
	int i, j;

	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 3; ++j) {
			output[i][j] = input[i][j] * 2;
		}
	}
}

int main()
{
	int input[2][3] = {
		{1, 2, 3},
		{4, 5, 6}
	};

	int output[2][3] = {0};
	times2(input, output);

	int i, j;
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 3; ++j) {
			printf("%d\t", output[i][j]);
		}
		printf("\n");
	}

	return 0;
}
