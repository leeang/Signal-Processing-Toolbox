#include <stdio.h>

static float bank_gain[20][59] = {0.0};

void calc_bank_gain(void)
{
	int bank_border[] = { 4, 361, 764, 1218, 1730, 2307, 2958, 3692, 4519, 5452, 6503, 7689, 9025, 10532, 12231, 14146, 16305, 18740, 21484, 24578, 28067, 32000 };

	float temp;

	int bank_num;
	for (bank_num = 0; bank_num < 20; bank_num++) {
		int x_length_inc = bank_border[bank_num+1] - bank_border[bank_num];
		int x_length_dec = bank_border[bank_num+2] - bank_border[bank_num+1];

		int offset = bank_border[bank_num]/125;

		int index = 1;
		int frequency_x4 = (index+offset) * 125;

		while (frequency_x4 <= bank_border[bank_num+1]) {
			temp = (float)(frequency_x4 - bank_border[bank_num]) / (float)x_length_inc;
			bank_gain[bank_num][index-1] = temp;
			index++;
			frequency_x4 = (index+offset) * 125;
		}

		while( frequency_x4 <= bank_border[bank_num+2] ) {
			temp = 1 - (float)(frequency_x4 - bank_border[bank_num+1]) / (float)x_length_dec;
			bank_gain[bank_num][index-1] = temp;
			index++;
			frequency_x4 = (index+offset) * 125;
		}
	}
}

int main()
{
	calc_bank_gain();

	int row, column;
	for (row = 0; row < 20; row++) {
		for (column = 0; column < 59; column++) {
			printf("%f\t", bank_gain[row][column]);
		}
		printf("\n");
	}

	return 0;
}
