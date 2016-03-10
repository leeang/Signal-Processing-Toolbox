#include <stdio.h>

static float bank_gain[20][58] = {0.0};

void calc_bank_gain(void)
{
	int bank_border[] = { 0, 3, 6, 10, 14, 18, 24, 29, 36, 44, 52, 61, 72, 84, 98, 113, 130, 150, 172, 197, 225, 256 };

	float temp;

	int bank_num;
	for (bank_num = 0; bank_num < 20; bank_num++) {
		int x_length_inc = bank_border[bank_num+1] - bank_border[bank_num];
		int x_length_dec = bank_border[bank_num+2] - bank_border[bank_num+1];

		int index;
		for (index = 1; index < x_length_inc; index++) {
			temp = (float)index / (float)x_length_inc;
			bank_gain[bank_num][index-1] = temp;
		}
		for (index = 0; index < x_length_dec; index++) {
			temp = 1 - (float)index / (float)x_length_dec;
			bank_gain[bank_num][x_length_inc + index-1] = temp;
		}
	}
}

int main()
{
	calc_bank_gain();

	int row, column;
	for (row = 0; row < 20; row++) {
		for (column = 0; column < 58; column++) {
			printf("%f\t", bank_gain[row][column]);
		}
		printf("\n");
	}

	return 0;
}
