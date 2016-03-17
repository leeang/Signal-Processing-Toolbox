#include <math.h>
#include <stdlib.h>

#include <fract.h>
#include <complex.h>
#include <filter.h>

#define WINDOW_LENGTH	512

#define BANK_NUM		20

#define TWIDDLE_STRIDE	1
#define DYNAMIC_SCALING	2

/* Mel filter */
int bank_border[] = { 4, 361, 764, 1218, 1730, 2307, 2958, 3692, 4519, 5452, 6503, 7689, 9025, 10532, 12231, 14146, 16305, 18740, 21484, 24578, 28067, 32000 };
fract32 bank_gain[477] = {0x0000};

/* FFT */
complex_fract16 fft_spectrum[WINDOW_LENGTH] = {0};
complex_fract16 twiddle_table[WINDOW_LENGTH/2] = {0};

float power[] = {
		0.382617, 0.401178, 0.615122, 0.908976, 1.581736, 
		3.061320, 8.522785, 57.028617, 179.534998, 16.551824, 
		5.598753, 3.723117, 2.075968, 2.675012, 10.366660, 
		271.429382, 36.911399, 6.761722, 2.991733, 2.250918, 
		1.441215, 1.123026, 1.403078, 0.750571, 0.226787, 
		0.341362, 0.321845, 0.347916, 0.383969, 0.508701, 
		1.174374, 0.164202, 0.045324, 0.084770, 0.092987, 
		0.084745, 0.085671, 0.119952, 0.209080, 0.205801, 
		0.092254, 0.068336, 0.082810, 0.073363, 0.074780, 
		0.089145, 0.029452, 0.044489, 0.053132, 0.053567, 
		0.037812, 0.055376, 0.069056, 0.165308, 0.035003, 
		0.020256, 0.022012, 0.035665, 0.024027, 0.016726, 
		0.059857, 0.148010, 0.117498, 0.064545, 0.028763, 
		0.036567, 0.039362, 0.021556, 0.098561, 0.007590, 
		0.021303, 0.047887, 0.051265, 0.062076, 0.037568, 
		0.192620, 0.380430, 0.338868, 0.106981, 0.057625, 
		0.013946, 0.135117, 0.151458, 0.143586, 0.362230, 
		0.017644, 0.014126, 0.025792, 0.053198, 0.042838, 
		0.054811, 0.017372, 0.179047, 0.087866, 0.010929, 
		0.044972, 0.091624, 0.017739, 0.200131, 0.357016, 
		0.008638, 0.007282, 0.033425, 0.008151, 0.012050, 
		0.007668, 0.017343, 0.042875, 0.031399, 0.014441, 
		0.014321, 0.026528, 0.012649, 0.002991, 0.013073, 
		0.003719, 0.022573, 0.013878, 0.007724, 0.020322, 
		0.015195, 0.001314, 0.046822, 0.007562, 0.003460, 
		0.008590, 0.015713, 0.012798, 0.012447, 0.007792, 
		0.035066, 0.029828, 0.018051, 0.004882, 0.017281, 
		0.038099, 0.040959, 0.053474, 0.006322, 0.020476, 
		0.036225, 0.014861, 0.102916, 0.482865, 0.593375, 
		0.795987, 0.429984, 0.131085, 0.041432, 0.026972, 
		0.001169, 0.136370, 0.098688, 0.008007, 0.004690, 
		0.006237, 0.009062, 0.002081, 0.002446, 0.065148, 
		0.025595, 0.072480, 0.039360, 0.004345, 0.013475, 
		0.007369, 0.084695, 0.130170, 0.062352, 0.105372, 
		0.104975, 0.019254, 0.049175, 0.062772, 0.052180, 
		0.004769, 0.017304, 0.009679, 0.002856, 0.004660, 
		0.006564, 0.011561, 0.004498, 0.007897, 0.005614, 
		0.009674, 0.007046, 0.001667, 0.007418, 0.022736, 
		0.012524, 0.008275, 0.001226, 0.003725, 0.010848, 
		0.014882, 0.009183, 0.002246, 0.003778, 0.010940, 
		0.014103, 0.007358, 0.001398, 0.005440, 0.010262, 
		0.009037, 0.003269, 0.002048, 0.006571, 0.011112, 
		0.011212, 0.003262, 0.000965, 0.008133, 0.008698, 
		0.008635, 0.002175, 0.001627, 0.011304, 0.025704, 
		0.001314, 0.001556, 0.003671, 0.024743, 0.007742, 
		0.009249, 0.004272, 0.063453, 0.012234, 0.012073, 
		0.003783, 0.015699, 0.018942, 0.005703, 0.007003, 
		0.008966, 0.002801, 0.013861, 0.002502, 0.003540, 
		0.005818, 0.006703, 0.001676, 0.012764, 0.004265, 
		0.007335, 0.002357, 0.007274, 0.009854, 0.009308, 
		0.003632, 0.005967, 0.005944, 0.005154, 0.004621, 
		0.004702, 0.004627, 0.004702, 0.004621, 0.005154, 
		0.005944, 0.005967, 0.003632, 0.009308, 0.009854, 
		0.007274, 0.002357, 0.007335, 0.004265, 0.012764, 
		0.001676, 0.006703, 0.005818, 0.003540, 0.002502, 
		0.013861, 0.002801, 0.008966, 0.007003, 0.005703, 
		0.018942, 0.015699, 0.003783, 0.012073, 0.012234, 
		0.063453, 0.004272, 0.009249, 0.007742, 0.024743, 
		0.003671, 0.001556, 0.001314, 0.025704, 0.011304, 
		0.001627, 0.002175, 0.008635, 0.008698, 0.008133, 
		0.000965, 0.003262, 0.011212, 0.011112, 0.006571, 
		0.002048, 0.003269, 0.009037, 0.010262, 0.005440, 
		0.001398, 0.007358, 0.014103, 0.010940, 0.003778, 
		0.002246, 0.009183, 0.014882, 0.010848, 0.003725, 
		0.001226, 0.008275, 0.012524, 0.022736, 0.007418, 
		0.001667, 0.007046, 0.009674, 0.005614, 0.007897, 
		0.004498, 0.011561, 0.006564, 0.004660, 0.002856, 
		0.009679, 0.017304, 0.004769, 0.052180, 0.062772, 
		0.049175, 0.019254, 0.104975, 0.105372, 0.062352, 
		0.130170, 0.084695, 0.007369, 0.013475, 0.004345, 
		0.039360, 0.072480, 0.025595, 0.065148, 0.002446, 
		0.002081, 0.009062, 0.006237, 0.004690, 0.008007, 
		0.098688, 0.136370, 0.001169, 0.026972, 0.041432, 
		0.131085, 0.429984, 0.795987, 0.593375, 0.482865, 
		0.102916, 0.014861, 0.036225, 0.020476, 0.006322, 
		0.053474, 0.040959, 0.038099, 0.017281, 0.004882, 
		0.018051, 0.029828, 0.035066, 0.007792, 0.012447, 
		0.012798, 0.015713, 0.008590, 0.003460, 0.007562, 
		0.046822, 0.001314, 0.015195, 0.020322, 0.007724, 
		0.013878, 0.022573, 0.003719, 0.013073, 0.002991, 
		0.012649, 0.026528, 0.014321, 0.014441, 0.031399, 
		0.042875, 0.017343, 0.007668, 0.012050, 0.008151, 
		0.033425, 0.007282, 0.008638, 0.357016, 0.200131, 
		0.017739, 0.091624, 0.044972, 0.010929, 0.087866, 
		0.179047, 0.017372, 0.054811, 0.042838, 0.053198, 
		0.025792, 0.014126, 0.017644, 0.362230, 0.143586, 
		0.151458, 0.135117, 0.013946, 0.057625, 0.106981, 
		0.338868, 0.380430, 0.192620, 0.037568, 0.062076, 
		0.051265, 0.047887, 0.021303, 0.007590, 0.098561, 
		0.021556, 0.039362, 0.036567, 0.028763, 0.064545, 
		0.117498, 0.148010, 0.059857, 0.016726, 0.024027, 
		0.035665, 0.022012, 0.020256, 0.035003, 0.165308, 
		0.069056, 0.055376, 0.037812, 0.053567, 0.053132, 
		0.044489, 0.029452, 0.089145, 0.074780, 0.073363, 
		0.082810, 0.068336, 0.092254, 0.205801, 0.209080, 
		0.119952, 0.085671, 0.084745, 0.092987, 0.084770, 
		0.045324, 0.164202, 1.174374, 0.508701, 0.383969, 
		0.347916, 0.321845, 0.341362, 0.226787, 0.750571, 
		1.403078, 1.123026, 1.441215, 2.250918, 2.991733, 
		6.761722, 36.911399, 271.429382, 10.366660, 2.675012, 
		2.075968, 3.723117, 5.598753, 16.551824, 179.534998, 
		57.028617, 8.522785, 3.061320, 1.581736, 0.908976, 
		0.615122, 0.401178
};

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

int main() {
	calc_bank_gain();
	twidfftrad2_fr16(twiddle_table, WINDOW_LENGTH);

	fract16 input_fr[WINDOW_LENGTH];
	int i;
	for (i = 0; i < WINDOW_LENGTH; ++i) {
		input_fr[i] = float_to_fr16(test_input[i]);
	}

	int block_exponent;
	rfft_fr16(input_fr, fft_spectrum, twiddle_table, TWIDDLE_STRIDE, WINDOW_LENGTH, &block_exponent, DYNAMIC_SCALING);

	fract32 power_spectrum[WINDOW_LENGTH];
	for (i = 0; i < WINDOW_LENGTH; ++i) {
		fract16 absolute = cabs_fr16(fft_spectrum[i]);
		power_spectrum[i] = mult_fr1x32(absolute, absolute);
	}

	float energy_melband[BANK_NUM] = {0.0};
	mel_filter(power_spectrum, energy_melband, block_exponent);

	int bank_num;
	for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
		printf("%f\t", energy_melband[bank_num]);
	}
	return 0;
}