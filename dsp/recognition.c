#include <cdefBF548.h>

#include "header.h"
#include "mel_filter.h"
#include "model.h"
#include "function.c"
#include "test_input.h"

int main() {
	/* Initialization */
	calc_shelving_coef();
	calc_hamming_coef();
	calc_bank_gain();
	twidfftrad2_fr32(twiddle_table, WINDOW_LENGTH);
	calc_dct_coef();
	/* /Initialization */

	int index;
	for (index = 0; index < SAMPLE_LENGTH; index++) {
		input_fr[index] = float_to_fr32(test_input[index]);
	}

	float clocks = clock();

	pre_emphasis(input_fr, SAMPLE_LENGTH);
	// printf("pre emphasis done\n");

	int frame_offset = 0;
	fract32 frame_data[WINDOW_LENGTH];
	int obs_length = 0;

	int frame_num;
	for (frame_num = 0; frame_num < FRAME_NUM; frame_num++) {
		for (index = 0; index < WINDOW_LENGTH; index++) {
			frame_data[index] = input_fr[index+frame_offset];
		}

		hamming(frame_data);

		float energy = calc_energy(frame_data, WINDOW_LENGTH);

		if (energy > ENERGY_THRSH) {
			float mfcc[FEAT_NUM] = {0.0};
			calc_mfcc(frame_data, mfcc_matrix[obs_length]);
			obs_length++;
		}
		frame_offset += WINDOW_LENGTH_HALF;
	}

	int result = get_result(obs_length, mfcc_matrix);

	clocks = clock() - clocks;
	float time_elapsed = clocks * 1000 / CLOCKS_PER_SEC;

	printf("Result: %s\n", word_string[result]);
	printf("Clocks: %d\n", clocks);
	printf("Time elapsed: %f ms\n", time_elapsed);

	return 0;
}
