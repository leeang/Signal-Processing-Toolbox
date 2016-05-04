#include <stdio.h>
#include <time.h>

#include "lms_data.c"

#define TOTAL_LENGTH	46336
#define LMS_LENGTH		128
#define LMS_STEP_SIZE	0.1
#define UPDATE_INTERVAL	4

float err[TOTAL_LENGTH] = { 0.0 };

int main() {
	float weights[LMS_LENGTH] = { 0.0 };

	float output;
	float square_sum = 0;
	float new_step_size;

	int n;
	int k;

	clock_t clocks = clock();

	for (n = 0; n < LMS_LENGTH; n++) {
		output = 0;
		for (k = 0; k <= n; k++) {
			output += weights[k] * noise_all[n-k];
		}

		err[n] = cmd_noise_all[n] - output;

		if (n%UPDATE_INTERVAL == 0) {
			square_sum += noise_all[n] * noise_all[n] * UPDATE_INTERVAL;
			new_step_size = LMS_STEP_SIZE / (1+square_sum);

			for (k = 0; k <= n; k++) {
				weights[k] += new_step_size * err[n] * noise_all[n-k];
			}
		}
	}

	for (n = LMS_LENGTH; n < TOTAL_LENGTH; n++) {
		output = 0;
		for (k = 0; k < LMS_LENGTH; k++) {
			output += weights[k] * noise_all[n-k];
		}

		err[n] = cmd_noise_all[n] - output;

		if (n%UPDATE_INTERVAL == 0) {
			square_sum = square_sum - noise_all[n-LMS_LENGTH] * noise_all[n-LMS_LENGTH] * UPDATE_INTERVAL + noise_all[n] * noise_all[n] * UPDATE_INTERVAL;
			new_step_size = LMS_STEP_SIZE / (1+square_sum);

			for (k = 0; k < LMS_LENGTH; k++) {
				weights[k] += new_step_size * err[n] * noise_all[n-k];
			}
		}
	}

	clocks = clock() - clocks;
	float time_elapsed = (float) clocks * 1000 / CLOCKS_PER_SEC;
	printf("Clocks: %d\n", clocks);
	printf("Time elapsed: %f ms\n", time_elapsed);

	for (n = 0; n < TOTAL_LENGTH; n++) {
		printf("%e\n", err[n]);
	}

	return 0;
}
