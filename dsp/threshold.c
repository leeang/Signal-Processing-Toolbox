#include <math.h>
#include <stdlib.h>

#include <fract.h>

#define YES				1
#define NO				0

#define WINDOW_LENGTH	256
#define PI				3.14159265358979323846

/* IIR filter */
#define BUFFER_SIZE		2
//define the buffer size
#define INDEX(CURRENT)	( ((CURRENT) + BUFFER_SIZE) % BUFFER_SIZE )
// if an index is negative, a specified position from the end of the array will be returned.
// e.g. given an array x[8], x[INDEX(-1)] and x[INDEX(7)] both refer to x[7].
/* /IIR filter */

/* zero crossing count */
#define IS_ZC(X, Y)		( (X) > 0 && (Y) < 0 || (X) < 0 && (Y) > 0 )

/* the coefficients of an IIR filter */
struct IIR_Coef {
	fract32 a1, a2;
	fract32 b0, b1, b2;
};

/* Global Coefficients */
struct IIR_Coef shelving_coef;
fract16 hamming_coef[WINDOW_LENGTH] = {0};
/* /Global Coefficients */

void calc_shelving_coef(void)
{
	shelving_coef.b0 = float_to_fr32(0.465464090346913);
	shelving_coef.b1 = float_to_fr32(-0.775712919663902);
	shelving_coef.b2 = float_to_fr32(0.341636074816722);
	shelving_coef.a1 = float_to_fr32(-0.380949232917015);
	shelving_coef.a2 = float_to_fr32(0.162336478416747);
}

void pre_emphasis(fract16 data[], int arr_length, char reset)
{
	static fract32 xBuffer[BUFFER_SIZE] = {0};
	// input buffer
	static fract32 yBuffer[BUFFER_SIZE] = {0};
	// output buffer

	static int current = 0;

	int index;

	if (reset == YES) {
		for (index = 0; index < BUFFER_SIZE; index++) {
			xBuffer[index] = yBuffer[index] = 0;
		}
		current = 0;
	}

	for (index = 0; index < arr_length; index++) {
		fract32 temp_b0 = mult_fr1x32x32(shelving_coef.b0, fr16_to_fr32(data[index]));
		fract32 temp_b1 = mult_fr1x32x32(shelving_coef.b1, xBuffer[INDEX(current-1)]);
		fract32 temp_b2 = mult_fr1x32x32(shelving_coef.b2, xBuffer[INDEX(current-2)]);
		fract32 temp_a1 = mult_fr1x32x32(shelving_coef.a1, yBuffer[INDEX(current-1)]);
		fract32 temp_a2 = mult_fr1x32x32(shelving_coef.a2, yBuffer[INDEX(current-2)]);

		fract32 temp_b = add_fr1x32(add_fr1x32(temp_b0, temp_b1), temp_b2);
		fract32 temp_a = add_fr1x32(temp_a1, temp_a2);
		fract32 temp = sub_fr1x32(temp_b, temp_a);

		yBuffer[current] = shl_fr1x32(temp, 2);

		xBuffer[current] = fr16_to_fr32(data[index]);		//input
		data[index] = fr32_to_fr16(yBuffer[current]);		//output

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
		hamming_coef[index] = float_to_fr16(w);
	}
}

void hamming(fract16 data[])
{
	int index;
	for (index = 0; index < WINDOW_LENGTH; index++) {
		data[index] = mult_fr1x16(data[index], hamming_coef[index]);
	}
}

float calc_energy(fract16 data[], int arr_length)
{
	int shift = 9;
	// shift = log_2(arr_length)

	fract32 energy_fr = float_to_fr32(0.0);

	int index;
	for (index = 0; index < arr_length; index++) {
		fract32 temp = mult_fr1x32(data[index], data[index]);
		temp = shl_fr1x32(temp, -shift);
		// right shift in case of overflow
		energy_fr = add_fr1x32(energy_fr, temp);
	}

	float energy = fr32_to_float(energy_fr) * (1<<shift);

	return energy;
}

int zc_count(fract16 data[], int arr_length)
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

float test_input[] = { -0.137543, -0.137787, -0.143433, -0.145386, -0.140533, 
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
		0.050812 };

int main() {
	fract16 data[WINDOW_LENGTH];

	int index;
	for (index = 0; index < WINDOW_LENGTH; index++) {
		data[index] = float_to_fr16(test_input[index]);
	}

	calc_shelving_coef();
	calc_hamming_coef();

	pre_emphasis(data, WINDOW_LENGTH, NO);
	hamming(data);

	float energy = calc_energy(data, WINDOW_LENGTH);
	printf("%f\n", energy);

	int zc_num = zc_count(data, WINDOW_LENGTH);
	printf("%d\n", zc_num);

	for (index = 0; index < WINDOW_LENGTH; ++index) {
		float test = fr16_to_float(data[index]);
		// printf("%f\n", fr16_to_float(data[index]));
	}

	return 0;
}
