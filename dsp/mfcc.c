#include <math.h>
#include <stdlib.h>

#include <fract.h>
#include <complex.h>
#include <filter.h>

#define PI	3.14159265358979323846

#define WINDOW_LENGTH	512

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

float test_input[] = {
		-0.137543, -0.137787, -0.143433, -0.145386, -0.140533, 
		-0.131836, -0.130005, -0.126801, -0.108826, -0.093018, 
		-0.074585, -0.052887, -0.035614, -0.029510, -0.013275, 
		0.013153, 0.029327, 0.041168, 0.049652, 0.060699, 
		0.064789, 0.072021, 0.076080, 0.071381, 0.063049, 
		0.062134, 0.059814, 0.046143, 0.036621, 0.030884, 
		0.026062, 0.019623, 0.015991, 0.004852, -0.007507, 
		-0.010712, -0.004272, 0.001923, 0.004547, 0.005524, 
		0.008057, 0.014252, 0.028870, 0.049316, 0.064392, 
		0.077362, 0.097534, 0.105408, 0.101166, 0.115448, 
		0.120636, 0.093292, 0.081360, 0.092560, 0.078979, 
		0.055603, 0.045441, 0.013153, -0.021576, -0.033783, 
		-0.034760, -0.053558, -0.076172, -0.097412, -0.115875, 
		-0.125793, -0.133392, -0.129913, -0.137756, -0.147583, 
		-0.145691, -0.129608, -0.128418, -0.127045, -0.101837, 
		-0.088165, -0.079590, -0.054779, -0.037262, -0.035095, 
		-0.014343, 0.018768, 0.031464, 0.043457, 0.049347, 
		0.057953, 0.065460, 0.075012, 0.086304, 0.081116, 
		0.065887, 0.057709, 0.063538, 0.047363, 0.037781, 
		0.035950, 0.027863, 0.015839, 0.011902, 0.004852, 
		-0.012665, -0.014771, -0.006683, 0.000427, -0.001923, 
		-0.004578, -0.004425, 0.000427, 0.016296, 0.035614, 
		0.052002, 0.055756, 0.073395, 0.096344, 0.100555, 
		0.096375, 0.118073, 0.109467, 0.078766, 0.082825, 
		0.093353, 0.067322, 0.044250, 0.040649, 0.004578, 
		-0.020752, -0.028870, -0.034424, -0.063263, -0.083862, 
		-0.096313, -0.105103, -0.121552, -0.133362, -0.128235, 
		-0.134949, -0.144653, -0.133820, -0.122589, -0.130646, 
		-0.120239, -0.089661, -0.081421, -0.077209, -0.052765, 
		-0.034485, -0.025970, -0.002991, 0.022156, 0.029266, 
		0.035614, 0.050110, 0.068939, 0.070770, 0.071594, 
		0.077332, 0.074554, 0.061493, 0.065277, 0.060516, 
		0.038422, 0.030914, 0.037537, 0.030548, 0.014252, 
		0.006622, -0.003265, -0.011658, -0.009491, 0.001282, 
		0.000122, -0.010590, -0.009705, 0.001221, 0.009888, 
		0.021301, 0.040466, 0.052368, 0.062500, 0.085876, 
		0.100433, 0.089264, 0.101166, 0.119202, 0.096527, 
		0.075195, 0.087921, 0.082367, 0.051178, 0.047791, 
		0.024658, -0.010773, -0.030457, -0.029388, -0.046509, 
		-0.068390, -0.091003, -0.106903, -0.112335, -0.127533, 
		-0.128632, -0.124298, -0.138947, -0.151245, -0.125458, 
		-0.116425, -0.124664, -0.103516, -0.082977, -0.085358, 
		-0.065216, -0.033356, -0.028503, -0.017792, 0.008698, 
		0.025452, 0.034912, 0.042664, 0.056854, 0.069000, 
		0.070129, 0.071960, 0.080322, 0.068909, 0.056091, 
		0.064178, 0.058502, 0.036804, 0.031128, 0.032654, 
		0.020172, 0.013428, 0.010040, -0.001129, -0.013580, 
		-0.011963, -0.005096, -0.003998, -0.009583, -0.007111, 
		0.002838, 0.007721, 0.019318, 0.037018, 0.044830, 
		0.050171, 0.076172, 0.100708, 0.092682, 0.093842, 
		0.108551, 0.086945, 0.067108, 0.083008, 0.090179, 
		0.050812, 0.037354, 0.020355, -0.005035, -0.024689, 
		-0.024506, -0.035645, -0.068634, -0.086212, -0.098267, 
		-0.099091, -0.117676, -0.118225, -0.118561, -0.130493, 
		-0.137421, -0.122009, -0.112549, -0.122314, -0.098572, 
		-0.077637, -0.078705, -0.068512, -0.045105, -0.028442, 
		-0.016724, 0.010071, 0.020844, 0.028381, 0.032959, 
		0.048615, 0.068939, 0.070160, 0.071899, 0.071625, 
		0.064575, 0.054230, 0.062561, 0.057465, 0.037262, 
		0.031372, 0.030853, 0.025513, 0.014252, 0.010010, 
		0.000641, -0.006439, -0.005646, -0.003021, -0.006561, 
		-0.015839, -0.011505, 0.003296, 0.012909, 0.016571, 
		0.024353, 0.032501, 0.039398, 0.069489, 0.097961, 
		0.094910, 0.078369, 0.099854, 0.100464, 0.068115, 
		0.073090, 0.096008, 0.069214, 0.036865, 0.040375, 
		0.007996, -0.026550, -0.020874, -0.012665, -0.045410, 
		-0.076752, -0.093475, -0.104187, -0.106873, -0.104431, 
		-0.104919, -0.120483, -0.146637, -0.134735, -0.111725, 
		-0.109436, -0.108154, -0.084015, -0.086334, -0.086884, 
		-0.054321, -0.035248, -0.026398, -0.005615, 0.014862, 
		0.015045, 0.025085, 0.041046, 0.058563, 0.068848, 
		0.068512, 0.068207, 0.062225, 0.054626, 0.060791, 
		0.068665, 0.050934, 0.031219, 0.031189, 0.029327, 
		0.023621, 0.018494, 0.011444, -0.006165, -0.008972, 
		-0.002228, 0.000000, -0.007416, -0.013885, -0.006561, 
		0.004150, 0.010712, 0.020874, 0.033173, 0.030060, 
		0.041534, 0.073700, 0.093781, 0.083344, 0.078796, 
		0.100922, 0.087402, 0.063568, 0.076630, 0.088989, 
		0.053955, 0.037140, 0.037079, -0.000763, -0.019867, 
		-0.016235, -0.019135, -0.052277, -0.073242, -0.090698, 
		-0.096100, -0.096985, -0.113495, -0.093323, -0.125488, 
		-0.139893, -0.123871, -0.114166, -0.107819, -0.106750, 
		-0.080658, -0.089478, -0.080231, -0.049347, -0.036926, 
		-0.030701, -0.001465, 0.011627, 0.015717, 0.029541, 
		0.034363, 0.058197, 0.069763, 0.067444, 0.068695, 
		0.060669, 0.052612, 0.063049, 0.071503, 0.049988, 
		0.037903, 0.033569, 0.028381, 0.023315, 0.022278, 
		0.015045, -0.003113, -0.003754, -0.003876, -0.003662, 
		-0.007172, -0.006317, 0.001953, 0.004272, 0.006561, 
		0.014862, 0.026764, 0.031799, 0.049316, 0.071747, 
		0.079773, 0.072083, 0.073608, 0.094513, 0.084900, 
		0.063507, 0.069458, 0.077576, 0.047821, 0.036011, 
		0.033844, -0.000763, -0.018188, -0.011841, -0.021454, 
		-0.053375, -0.069794, -0.082855, -0.090363, -0.090790, 
		-0.099976, -0.105713, -0.116119, -0.128021, -0.119965, 
		-0.100952, -0.106018, -0.104889, -0.083221, -0.082306, 
		-0.075012, -0.050690, -0.034058, -0.027527, -0.007385, 
		0.005707, 0.013519, 0.025970, 0.036896, 0.053589, 
		0.063416, 0.058502, 0.056213, 0.060059, 0.056732, 
		0.058350, 0.060425, 0.042664, 0.028931, 0.032379, 
		0.033051, 0.022980, 0.016388, 0.007599, -0.002563, 
		-0.003052, -0.003021, -0.004272, -0.006195, -0.006683, 
		-0.001923, 0.004242
};

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

	fract32 input_fr[WINDOW_LENGTH];
	int i;
	for (i = 0; i < WINDOW_LENGTH; ++i) {
		input_fr[i] = float_to_fr32(test_input[i]);
	}

	pre_emphasis(input_fr, WINDOW_LENGTH);
	hamming(input_fr);

	float energy = calc_energy(input_fr, WINDOW_LENGTH);
	printf("%f\n", energy);

	float mfcc[MFCC_NUM] = {0.0};
	calc_mfcc(input_fr, mfcc);

	int mfcc_num;
	for (mfcc_num = 0; mfcc_num < MFCC_NUM; mfcc_num++) {
		printf("%f\t", mfcc[mfcc_num]);
	}

	return 0;
}
