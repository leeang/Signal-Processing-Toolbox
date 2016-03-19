#include <math.h>
#include <stdlib.h>

#include <fract.h>
#include <complex.h>
#include <filter.h>

#define PI	3.14159265358979323846

#define TOTAL_LENGTH	48000
#define WINDOW_LENGTH	512
#define FRAME_NUM		180

/* IIR filter */
#define BUFFER_SIZE		2
	//define the buffer size
#define INDEX(CURRENT)	( ((CURRENT) + BUFFER_SIZE) % BUFFER_SIZE )
	// if an index is negative, a specified position from the end of the array will be returned.
	// e.g. given an array x[8], x[INDEX(-1)] and x[INDEX(7)] both refer to x[7].
/* /IIR filter */

/* zero crossing count */
#define IS_ZC(X, Y)		( (X) > 0 && (Y) < 0 || (X) < 0 && (Y) > 0 )

/* FFT */
#define TWIDDLE_STRIDE	1
#define DYNAMIC_SCALING	2

/* Mel filter */
#define BANK_NUM		20
#define MFCC_NUM		12

/* the coefficients of an IIR filter */
struct IIR_Coef {
	fract32 a1, a2;
	fract32 b0, b1, b2;
};

/* Global Coefficients */
struct IIR_Coef shelving_coef;
fract32 hamming_coef[WINDOW_LENGTH] = {0};
/* /Global Coefficients */

/* FFT */
complex_fract32 fft_spectrum[WINDOW_LENGTH] = {0};
complex_fract32 twiddle_table[WINDOW_LENGTH/2] = {0};

/* Mel filter */
int bank_border[] = { 4, 361, 764, 1218, 1730, 2307, 2958, 3692, 4519, 5452, 6503, 7689, 9025, 10532, 12231, 14146, 16305, 18740, 21484, 24578, 28067, 32000 };
fract32 bank_gain[477] = {0x0000};

/* discrete cosine transform */
float dct_coef[MFCC_NUM][BANK_NUM] = {0.0};

section("sdram0") fract32 input_fr[TOTAL_LENGTH];
section("sdram0") fract32 voiced_cepstrum[FRAME_NUM][MFCC_NUM] = {0.0};;

#include "test_input.h"

void calc_shelving_coef(void)
{
	shelving_coef.b0 = float_to_fr32(0.465464090346913);
	shelving_coef.b1 = float_to_fr32(-0.775712919663902);
	shelving_coef.b2 = float_to_fr32(0.341636074816722);
	shelving_coef.a1 = float_to_fr32(-0.380949232917015);
	shelving_coef.a2 = float_to_fr32(0.162336478416747);
}

void pre_emphasis(fract32 data[], int arr_length)
{
	fract32 xBuffer[BUFFER_SIZE] = {0};
	// input buffer
	fract32 yBuffer[BUFFER_SIZE] = {0};
	// output buffer

	int current = 0;

	int index;

	for (index = 0; index < arr_length; index++) {
		fract32 temp_b0 = mult_fr1x32x32(shelving_coef.b0, data[index]);
		fract32 temp_b1 = mult_fr1x32x32(shelving_coef.b1, xBuffer[INDEX(current-1)]);
		fract32 temp_b2 = mult_fr1x32x32(shelving_coef.b2, xBuffer[INDEX(current-2)]);
		fract32 temp_a1 = mult_fr1x32x32(shelving_coef.a1, yBuffer[INDEX(current-1)]);
		fract32 temp_a2 = mult_fr1x32x32(shelving_coef.a2, yBuffer[INDEX(current-2)]);

		fract32 temp_b = add_fr1x32(add_fr1x32(temp_b0, temp_b1), temp_b2);
		fract32 temp_a = add_fr1x32(temp_a1, temp_a2);
		fract32 temp = sub_fr1x32(temp_b, temp_a);

		yBuffer[current] = shl_fr1x32(temp, 2);

		xBuffer[current] = data[index];		//input
		data[index] = yBuffer[current];		//output

		current++;
		current %= BUFFER_SIZE;
	}
}

void calc_hamming_coef(void)
{
	int index;
	float w;
	for (index = 0; index < WINDOW_LENGTH; index++) {
		w = 0.54 - 0.46 * cosf( 2 * PI * (float) index / (float) (WINDOW_LENGTH-1) );
		hamming_coef[index] = float_to_fr32(w);
	}
}

void hamming(fract32 data[])
{
	int index;
	for (index = 0; index < WINDOW_LENGTH; index++) {
		data[index] = mult_fr1x32x32(data[index], hamming_coef[index]);
	}
}

float calc_energy(fract32 data[], int arr_length)
{
	int shift = 9;
	// shift = log_2(arr_length)

	fract32 energy_fr = float_to_fr32(0.0);

	int index;
	for (index = 0; index < arr_length; index++) {
		fract32 temp = mult_fr1x32x32(data[index], data[index]);
		temp = shl_fr1x32(temp, -shift);
		// right shift in case of overflow
		energy_fr = add_fr1x32(energy_fr, temp);
	}

	float energy = fr32_to_float(energy_fr) * (1<<shift);

	return energy;
}

int zc_count(fract32 data[], int arr_length)
{
	int zc_num = 0;

	int index;
	for (index = 1; index < arr_length; index++) {
		if ( IS_ZC(data[index], data[index-1]) ) {
			zc_num += 1;
		}
	}
	return zc_num;
}

void calc_bank_gain(void)
{
	float temp;
	int bank_gain_index = 0;

	int bank_num;
	for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
		int x_length_inc = bank_border[bank_num+1] - bank_border[bank_num];
		int x_length_dec = bank_border[bank_num+2] - bank_border[bank_num+1];

		int offset = bank_border[bank_num] / 125;

		int index = 1;
		int frequency_x4 = (index+offset) * 125;

		while (frequency_x4 <= bank_border[bank_num+1]) {
			temp = (float)(frequency_x4 - bank_border[bank_num]) / (float)x_length_inc;
			bank_gain[bank_gain_index] = float_to_fr32(temp);
			index++;
			frequency_x4 = (index+offset) * 125;
			bank_gain_index++;
		}

		while( frequency_x4 <= bank_border[bank_num+2] ) {
			temp = 1 - (float)(frequency_x4 - bank_border[bank_num+1]) / (float)x_length_dec;
			bank_gain[bank_gain_index] = float_to_fr32(temp);
			index++;
			frequency_x4 = (index+offset) * 125;
			bank_gain_index++;
		}
	}
}

void mel_filter(fract32 power_fr[], float energy_melband[], int block_exponent)
{
	int scale = 1 << block_exponent*2;

	int bank_num;
	for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
		int offset = bank_border[bank_num]/125;

		int L = bank_border[bank_num+2]/125 - bank_border[bank_num]/125;

		int bank_gain_offset;
		switch (bank_num) {
			case 0:
				bank_gain_offset = bank_border[0]/125;
				break;
			case 1:
				bank_gain_offset = bank_border[2]/125 - bank_border[0]/125;
				break;
			default:
				bank_gain_offset = bank_border[bank_num+1]/125 + bank_border[bank_num]/125 - bank_border[1]/125 - bank_border[0]/125;
		}

		int index;
		for (index = 0; index < L; index++) {
			fract32 temp = mult_fr1x32x32(power_fr[index+offset+1], bank_gain[index+bank_gain_offset]);
			energy_melband[bank_num] = energy_melband[bank_num] + fr32_to_float(temp);
		}

		energy_melband[bank_num] = energy_melband[bank_num] * scale;
		energy_melband[bank_num] = log10f(energy_melband[bank_num]);
	}
}

void calc_dct_coef(void)
{
	float scale = sqrtf(2.0 / BANK_NUM);

	int mfcc_num, bank_num;

	for (mfcc_num = 0; mfcc_num < MFCC_NUM; mfcc_num++) {
		for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
			dct_coef[mfcc_num][bank_num] = cosf(PI * (bank_num+0.5) * (float) mfcc_num / (float) BANK_NUM) * scale;
		}
	}
}

void discrete_cosine_transform(float energy[], float out_mfcc[])
{
	int mfcc_num, bank_num;

	for (mfcc_num = 0; mfcc_num < MFCC_NUM; mfcc_num++) {
		float sum = 0;
		for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
			sum += energy[bank_num] * dct_coef[mfcc_num][bank_num];
		}
		out_mfcc[mfcc_num] = sum;
	}
}

void calc_mfcc(fract32 input_fr[], float out_mfcc[])
{
	int i;

	int block_exponent;
	rfft_fr32(input_fr, fft_spectrum, twiddle_table, TWIDDLE_STRIDE, WINDOW_LENGTH, &block_exponent, DYNAMIC_SCALING);

	fract32 power_spectrum[WINDOW_LENGTH];
	for (i = 0; i < WINDOW_LENGTH; ++i) {
		fract32 absolute = cabs_fr32(fft_spectrum[i]);
		power_spectrum[i] = mult_fr1x32x32(absolute, absolute);
	}

	float energy_melband[BANK_NUM] = {0.0};
	mel_filter(power_spectrum, energy_melband, block_exponent);
	discrete_cosine_transform(energy_melband, out_mfcc);
}

int main() {
	/* Initialization */
	calc_shelving_coef();
	calc_hamming_coef();
	calc_bank_gain();
	twidfftrad2_fr32(twiddle_table, WINDOW_LENGTH);
	calc_dct_coef();
	/* /Initialization */

	int index;
	for (index = 0; index < TOTAL_LENGTH; index++) {
		input_fr[index] = float_to_fr32(test_input[index]);
	}

	pre_emphasis(input_fr, TOTAL_LENGTH);
	// printf("pre emphasis done\n");

	int frame_offset = 0;
	fract32 frame_data[WINDOW_LENGTH];
	int voiced_cepstrum_index = 0;

	int frame_num;
	int mfcc_num;
	for (frame_num = 0; frame_num < FRAME_NUM; frame_num++) {
		for (index = 0; index < WINDOW_LENGTH; index++) {
			frame_data[index] = input_fr[index+frame_offset];
		}

		hamming(frame_data);

		float energy = calc_energy(frame_data, WINDOW_LENGTH);

		if (energy > 0.1) {
			float mfcc[MFCC_NUM] = {0.0};
			calc_mfcc(frame_data, mfcc);

			for (mfcc_num = 0; mfcc_num < MFCC_NUM; mfcc_num++) {
				voiced_cepstrum[voiced_cepstrum_index][mfcc_num] = mfcc[mfcc_num];
			}
			voiced_cepstrum_index++;
		}
		frame_offset += WINDOW_LENGTH/2;
	}

	for (frame_num = 0; frame_num < voiced_cepstrum_index; frame_num++) {
		for (mfcc_num = 0; mfcc_num < MFCC_NUM; mfcc_num++) {
			printf("%f\t", voiced_cepstrum[frame_num][mfcc_num]);
		}
		printf("\n");
	}

	return 0;
}
