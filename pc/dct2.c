#include <stdio.h>
#include <math.h>

#define PI	3.14159265358979323846

#define BANK_NUM		20
#define MFCC_COEF_NUM	12

float dct_coef[MFCC_COEF_NUM][BANK_NUM] = {0.0};

void calc_mfcc_coef(void)
{
	int mfcc_coef_num, bank_num;

	for (mfcc_coef_num = 0; mfcc_coef_num < MFCC_COEF_NUM; mfcc_coef_num++) {
		for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
			dct_coef[mfcc_coef_num][bank_num] = cosf(PI * (bank_num+0.5) * mfcc_coef_num / (float) BANK_NUM);
		}
	}
}

int main()
{
	calc_mfcc_coef();

	int mfcc_coef_num, bank_num;
	for (mfcc_coef_num = 0; mfcc_coef_num < MFCC_COEF_NUM; mfcc_coef_num++) {
		for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
			printf("%f\t", dct_coef[mfcc_coef_num][bank_num]);
		}
		printf("\n");
	}
}
