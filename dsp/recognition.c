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

		if (energy > 0.1) {
			float mfcc[FEAT_NUM] = {0.0};
			calc_mfcc(frame_data, mfcc_matrix[obs_length]);
			obs_length++;
		}
		frame_offset += WINDOW_LENGTH/2;
	}

	int result = get_result(obs_length, mfcc_matrix);
	printf("Result: %s\n", word_string[result]);

	return 0;
}
