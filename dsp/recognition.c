#include <cdefBF548.h>

#include "header.h"
#include "mel_filter.h"
#include "model.h"
#include "function.c"
#include "cmd.h"
#include "lms_data.h"

section("sdram0") float cmd_float[SAMPLE_LENGTH];

int main() {
	/* Initialization */
	calc_shelving_coef();
	calc_hamming_coef();
	calc_bank_gain();
	twidfftrad2_fr32(twiddle_table, WINDOW_LENGTH);
	calc_dct_coef();
	energy_threshold = float_to_fr32(ENERGY_THRESHOLD / (float) (1 << ENERGY_RIGHT_SHIFT));
	/* /Initialization */

	int chunk_index = 0;
	for (chunk_index = 0; chunk_index < CHUNCK_NUM; chunk_index++) {
		int offset = chunk_index * CHUNCK_LENGTH;

		int index;
		for (index = 0; index < CHUNCK_LENGTH; index++) {
			cmd_noise[index] = cmd_noise_all[offset + index];
			// cmd_noise[index] = cmd[offset + index];
			noise[index] = noise_all[offset + index];
		}

		lms(chunk_index);
		// lms_pass(chunk_index);
	}

	int index;
	for (index = 0; index < SAMPLE_LENGTH; index++) {
		cmd_float[index] = fr32_to_float(cmd_fr32[index]);
	}

	clock_t clocks = clock();

	pre_emphasis(cmd_fr32, SAMPLE_LENGTH);
	// printf("pre emphasis done\n");

	int frame_offset = 0;
	fract32 frame_data[WINDOW_LENGTH];
	int obs_length = 0;

	int frame_num;
	for (frame_num = 0; frame_num < FRAME_NUM; frame_num++) {
		for (index = 0; index < WINDOW_LENGTH; index++) {
			frame_data[index] = cmd_fr32[index+frame_offset];
		}

		hamming(frame_data);

		fract32 energy = calc_energy(frame_data);

		if (energy > energy_threshold) {
			float mfcc[FEAT_NUM] = {0.0};
			calc_mfcc(frame_data, mfcc_matrix[obs_length]);
			obs_length++;
		}
		frame_offset += WINDOW_LENGTH_HALF;
	}

	int result = get_result(obs_length, mfcc_matrix);

	clocks = clock() - clocks;
	float time_elapsed = (float) clocks * 1000 / CLOCKS_PER_SEC;

	printf("Result: %s\n", word_string[result]);
	printf("Clocks: %d\n", clocks);
	printf("Time elapsed: %f ms\n", time_elapsed);

	return 0;
}
