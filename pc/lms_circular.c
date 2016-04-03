#include <stdio.h>

#include "lms_data.c"

#define TOTAL_LENGTH	48000
#define LMS_LENGTH		300
#define LMS_STEP_SIZE	0.04

double err[TOTAL_LENGTH] = { 0.0 };
double weights[LMS_LENGTH] = { 0.0 };
double noise_buffer[LMS_LENGTH] = { 0.0 };
double square_sum = 0;

int main() {
	double output;
	double new_step_size;

	int n;
	int k;

	for (n = 0; n < LMS_LENGTH; n++) {
		output = 0;
		for (k = 0; k <= n; k++) {
			output += weights[k] * noise[n-k];
		}
		for (k = n+1; k < LMS_LENGTH; k++) {
			output += weights[k] * noise_buffer[n-k+LMS_LENGTH];
		}

		err[n] = cmd_noise[n] - output;

		square_sum = square_sum - noise_buffer[n] * noise_buffer[n] + noise[n] * noise[n];
		new_step_size = LMS_STEP_SIZE / (1+square_sum);

		for (k = 0; k <= n; k++) {
			weights[k] += new_step_size * err[n] * noise[n-k];
		}
		for (k = n+1; k < LMS_LENGTH; k++) {
			weights[k] += new_step_size * err[n] * noise_buffer[n-k+LMS_LENGTH];
		}
	}

	for (n = LMS_LENGTH; n < TOTAL_LENGTH; n++) {
		output = 0;
		for (k = 0; k < LMS_LENGTH; k++) {
			output += weights[k] * noise[n-k];
		}

		err[n] = cmd_noise[n] - output;

		square_sum = square_sum - noise[n-LMS_LENGTH] * noise[n-LMS_LENGTH] + noise[n] * noise[n];
		new_step_size = LMS_STEP_SIZE / (1+square_sum);

		for (k = 0; k < LMS_LENGTH; k++) {
			weights[k] += new_step_size * err[n] * noise[n-k];
		}
	}

	for (n = 0; n < LMS_LENGTH; n++) {
		noise_buffer[n] = noise[TOTAL_LENGTH-LMS_LENGTH+n];
	}

	for (n = 0; n < TOTAL_LENGTH; n++) {
		printf("%e\n", err[n]);
	}

	return 0;
}
