#include <stdio.h>

#include "lms_data.c"

#define SAMPLE_LENGTH	48000
#define LMS_LENGTH		300
#define LMS_STEP_SIZE	0.04

double weights[LMS_LENGTH] = { 0.0 };
double noise_buffer[LMS_LENGTH] = { 0.0 };
double lms_square_sum = 0;

double err1[SAMPLE_LENGTH] = {0};
double err2[SAMPLE_LENGTH] = {0};

void lms_circular(float cmd_noise_in[], float noise_in[], double err_out[], int arr_length) {
	double output;
	double new_step_size;

	int n, k;

	for (n = 0; n < LMS_LENGTH; n++) {
		output = 0;
		for (k = 0; k <= n; k++) {
			output += weights[k] * noise_in[n-k];
		}
		for (k = n+1; k < LMS_LENGTH; k++) {
			output += weights[k] * noise_buffer[n-k+LMS_LENGTH];
		}

		err_out[n] = cmd_noise_in[n] - output;

		lms_square_sum = lms_square_sum - noise_buffer[n] * noise_buffer[n] + noise_in[n] * noise_in[n];
		new_step_size = LMS_STEP_SIZE / (1+lms_square_sum);

		for (k = 0; k <= n; k++) {
			weights[k] += new_step_size * err_out[n] * noise_in[n-k];
		}
		for (k = n+1; k < LMS_LENGTH; k++) {
			weights[k] += new_step_size * err_out[n] * noise_buffer[n-k+LMS_LENGTH];
		}
	}

	for (n = LMS_LENGTH; n < arr_length; n++) {
		output = 0;
		for (k = 0; k < LMS_LENGTH; k++) {
			output += weights[k] * noise_in[n-k];
		}

		err_out[n] = cmd_noise_in[n] - output;

		lms_square_sum = lms_square_sum - noise_in[n-LMS_LENGTH] * noise_in[n-LMS_LENGTH] + noise_in[n] * noise_in[n];
		new_step_size = LMS_STEP_SIZE / (1+lms_square_sum);

		for (k = 0; k < LMS_LENGTH; k++) {
			weights[k] += new_step_size * err_out[n] * noise_in[n-k];
		}
	}

	for (n = 0; n < LMS_LENGTH; n++) {
		noise_buffer[n] = noise_in[arr_length-LMS_LENGTH+n];
	}
}

int main() {
	lms_circular(cmd_noise, noise, err1, SAMPLE_LENGTH);
	lms_circular(cmd_noise, noise, err2, SAMPLE_LENGTH);

	int n;
	for (n = 0; n < SAMPLE_LENGTH; n++) {
		printf("%e\n", err1[n]);
	}
	for (n = 0; n < SAMPLE_LENGTH; n++) {
		printf("%e\n", err2[n]);
	}

	return 0;
}
