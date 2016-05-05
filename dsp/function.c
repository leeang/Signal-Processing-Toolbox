void lms(int chunk_index)
{
	static float square_sum = 0;
	static int current = 0;

	int offset = chunk_index * CHUNCK_LENGTH;
	int update_counter = UPDATE_INTERVAL;

	int n;
	int k;

	for (n = 0; n < CHUNCK_LENGTH; n++) {
		float noise_last = noise_buffer[current];
		// keep the last element that will be overwritten
		noise_buffer[current] = noise[n];
		// overwrite the last element by newest data point

		float output = 0;
		for (k = 0; k < LMS_LENGTH; k++) {
			output += weights[k] * noise_buffer[MOD(current-k)];
			// MOD(CURRENT) macro is explained in header.h
		}

		float err = cmd_noise[n] - output;
		cmd_fr32[n+offset] = float_to_fr32(err);

		if (update_counter == UPDATE_INTERVAL) {
			square_sum = square_sum + (noise_buffer[current] * noise_buffer[current] - noise_last * noise_last) * UPDATE_INTERVAL;
			float new_step_size = LMS_STEP_SIZE / (1 + square_sum);

			for (k = 0; k < LMS_LENGTH; k++) {
				weights[k] += new_step_size * err * noise_buffer[MOD(current-k)];
			}

			update_counter = 0;
		}
		update_counter++;

		current = (current + 1) & LMS_LENGTH_MASK;
		// current increments from 0 to LMS_LENGTH-1 and then starts from 0 again.
	}
}

void lms_pass(int chunk_index)
{
	int offset = chunk_index * CHUNCK_LENGTH;

	int n;
	for (n = 0; n < CHUNCK_LENGTH; n++) {
		cmd_fr32[n + offset] = float_to_fr32(cmd_noise[n]);
	}
}

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
	fract32 x1 = 0, x2 = 0;
	// input buffer
	fract32 y1 = 0, y2 = 0;
	// output buffer

	int index;
	for (index = 0; index < arr_length; index++) {
		fract32 temp_b0 = multr_fr1x32x32(shelving_coef.b0, data[index]);
		fract32 temp_b1 = multr_fr1x32x32(shelving_coef.b1, x1);
		fract32 temp_b2 = multr_fr1x32x32(shelving_coef.b2, x2);
		fract32 temp_a1 = multr_fr1x32x32(shelving_coef.a1, y1);
		fract32 temp_a2 = multr_fr1x32x32(shelving_coef.a2, y2);

		fract32 temp_b = add_fr1x32(add_fr1x32(temp_b0, temp_b1), temp_b2);
		fract32 temp_a = add_fr1x32(temp_a1, temp_a2);
		fract32 temp = sub_fr1x32(temp_b, temp_a);

		x2 = x1;
		x1 = data[index];		//input

		data[index] = shl_fr1x32(temp, 2);

		y2 = y1;
		y1 = data[index];		//output
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
		data[index] = multr_fr1x32x32(data[index], hamming_coef[index]);
	}
}

fract32 calc_energy(fract32 data[])
{
	fract32 energy = 0;

	int index;
	for (index = 0; index < WINDOW_LENGTH; index++) {
		fract32 power = multr_fr1x32x32(data[index], data[index]);

		power = shr_fr1x32(power, ENERGY_RIGHT_SHIFT);
		// right shift before accumulation in case of overflow

		energy = add_fr1x32(energy, power);
	}

	return energy;
}

/*
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
*/

void calc_bank_gain(void)
{
	int index;
	for (index = 0; index < BANK_GAIN_LENGTH; index++) {
		bank_gain[index] = float_to_fr32(bank_gain_float[index]);
	}
}

void mel_filter(fract32 power[], float energy[], int block_exponent)
{
	int scale = 1 << block_exponent*2;

	int bank_num;
	for (bank_num = 0; bank_num < BANK_NUM; bank_num++) {
		float energy_float = 0;

		int index;
		for (index = 0; index < fft_index_length[bank_num]; index++) {
			fract32 filtered_power = multr_fr1x32x32(
				power[index + fft_index_offset[bank_num]],
				bank_gain[index + bank_gain_index_offset[bank_num]]
			);
			energy_float += fr32_to_float(filtered_power);
		}

		energy[bank_num] = log10f(energy_float * scale);
	}
}

void calc_dct_coef(void)
{
	float scale = sqrtf(2.0 / BANK_NUM);

	int feat_num, bank_num;

	for (feat_num = 0; feat_num < FEAT_NUM; feat_num++) {
		for (bank_num = 0; bank_num < BANK_NUM_HALF; bank_num++) {
			dct_coef[feat_num][bank_num] = cosf(PI * (bank_num+0.5) * (float) feat_num / (float) BANK_NUM) * scale;
		}
	}
}

void discrete_cosine_transform(float energy[], float mfcc_row[])
{
	int feat_num, bank_num;
	float sum;

	for (feat_num = 0; feat_num < FEAT_NUM; feat_num+=2) {
		sum = 0;
		for (bank_num = 0; bank_num < BANK_NUM_HALF; bank_num++) {
			sum += (energy[bank_num] + energy[BANK_NUM-1-bank_num]) * dct_coef[feat_num][bank_num];
		}
		mfcc_row[feat_num] = sum;
	}

	for (feat_num = 1; feat_num < FEAT_NUM; feat_num+=2) {
		sum = 0;
		for (bank_num = 0; bank_num < BANK_NUM_HALF; bank_num++) {
			sum += (energy[bank_num] - energy[BANK_NUM-1-bank_num]) * dct_coef[feat_num][bank_num];
		}
		mfcc_row[feat_num] = sum;
	}
}

void calc_mfcc(fract32 frame_data_point[], float mfcc_row[])
{
	int i;

	int block_exponent;
	rfft_fr32(frame_data_point, fft_spectrum, twiddle_table, TWIDDLE_STRIDE, WINDOW_LENGTH, &block_exponent, DYNAMIC_SCALING);

	fract32 power_spectrum[WINDOW_LENGTH_HALF];
	for (i = 0; i < WINDOW_LENGTH_HALF; ++i) {
		fract32 absolute = cabs_fr32(fft_spectrum[i]);
		power_spectrum[i] = multr_fr1x32x32(absolute, absolute);
	}

	float bank_energy[BANK_NUM] = {0.0};
	mel_filter(power_spectrum, bank_energy, block_exponent);
	discrete_cosine_transform(bank_energy, mfcc_row);
}

void calc_emis(int obs_length, float obs[][FEAT_NUM], int word_index, float *ptr_to_emis)
{
	int state_index;
	int obs_index;
	int feat_num;

	for (obs_index = 0; obs_index < obs_length; obs_index++) {
		for (state_index = 0; state_index < STATE_NUM; state_index++) {

			float trans = 0;
			for (feat_num = 0; feat_num < FEAT_NUM; feat_num++) {
				float obs_minus_mu = obs[obs_index][feat_num] - mu[word_index][state_index][feat_num];

				trans += obs_minus_mu * inv_Var[word_index][state_index][feat_num] * obs_minus_mu;
			}

			float exp_part = trans;
			// exp_part = exp(-0.5 * trans);
			// take logarithm
			// inv_Var[word_index][state_index][feat_num] has been multiplied by -0.5 in MATLAB before export

			EMIS(obs_index, state_index) = det_part[word_index][state_index] + exp_part;
			// det_part = 1 / ( sqrt(pow((2*PI), FEAT_NUM) * det_var[state_index]) );
			// take logarithm
			// preceding 2 steps have been done in MATLAB before export
			// multiplication becomes addition

		}
	}
}

float viterbi(int obs_length, float obs[][FEAT_NUM], int word_index)
{
	float *ptr_to_emis;
	ptr_to_emis = (float *) calloc(obs_length * STATE_NUM, sizeof(float));

	float *ptr_to_phi;
	ptr_to_phi = (float *) calloc(obs_length * STATE_NUM, sizeof(float));

	calc_emis(obs_length, obs, word_index, ptr_to_emis);

	// initialization
	PHI(0, 0) = EMIS(0, 0);


	int obs_index;
	int statei, statej;
	float max_phi_plus_trans;

	// recursion
	for (obs_index = 1; obs_index < obs_length; obs_index++) {

		// find max( PHI(obs_index-1, statei) + trans[word_index][statei][statej] ) when statei varies

		// statej = 0
		max_phi_plus_trans = -Inf;
		float phi_plus_trans = PHI(obs_index-1, 0) + trans[word_index][0][0];

		if (phi_plus_trans > max_phi_plus_trans){
			max_phi_plus_trans = phi_plus_trans;
		}

		PHI(obs_index, 0) = max_phi_plus_trans + EMIS(obs_index, 0);

		// statej = 1:STATE_NUM-1
		for (statej = 1; statej < STATE_NUM; statej++) {

			max_phi_plus_trans = -Inf;

			// statei = 0;
			// while (statei < STATE_NUM) {
			// optimization considering the shape of trans matrix
			statei = statej - 1;
			while (statei <= statej) {
				float phi_plus_trans = PHI(obs_index-1, statei) + trans[word_index][statei][statej];

				if (phi_plus_trans > max_phi_plus_trans){
					max_phi_plus_trans = phi_plus_trans;
				}
				statei++;
			}

			PHI(obs_index, statej) = max_phi_plus_trans + EMIS(obs_index, statej);

		}
	}

	float probability = -Inf;
	// find max( PHI(obs_length-1, statei) ) when statei changes
	for (statei = 0; statei < STATE_NUM; statei++) {
		if (PHI(obs_length-1, statei) > probability){
			probability = PHI(obs_length-1, statei);
		}
	}

	free(ptr_to_emis);
	free(ptr_to_phi);

	return probability;
}

int get_result(int obs_length, float obs[][FEAT_NUM])
{
	float P_word_max = -Inf;
	int result;

	int word_index;
	for (word_index=0; word_index < WORD_NUM ; word_index++) {
		float P_word = viterbi(obs_length, obs, word_index);

		if (P_word > P_word_max) {
			P_word_max = P_word;
			result = word_index;
		}
		// printf("%f\t", P_word);
	}
	// printf("\n");

	if (fabs(P_word_max) < 0.000001) {
		result = 27;	// "louder please"
	}

	return result;
}

void init_gpio(void)
{
	*pPORTG_FER			= 0x0000;		// Setup PG4 - PG9 for LEDs
	*pPORTG_MUX			= 0x0000;		// Turn all LEDs on
	*pPORTG_DIR_SET		= 0x0FC0;		// Setup port for output
	*pPORTG_CLEAR		= 0x0FC0;		// Turn all LEDs off
}

void control_gpio(int result)
{
	static unsigned short flag = 1;

	unsigned short gpio_register;
	if (result >= 0 && result < WORD_NUM) {
		gpio_register = result + 1;
	} else {
		gpio_register = 0;
	}

	gpio_register <<= 6;
	gpio_register |= (flag << 11);
	*pPORTG_CLEAR = 0x0FC0;
	*pPORTG_SET = gpio_register;

	flag = 1 - flag;
}
