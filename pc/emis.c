#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define FEAT_NUM		12
#define STATE_NUM		5
#define Inf				10000

#define EMIS(OBS_INDEX, STATE_INDEX)	*(ptr_to_emis + OBS_INDEX * STATE_NUM + STATE_INDEX)
#define PHI(OBS_INDEX, STATE_INDEX)		*(ptr_to_phi + STATE_NUM*OBS_INDEX + STATE_INDEX)

float test_input[18][FEAT_NUM] = {
	{-16.681906, 6.087717, 2.390434, 0.232071, -1.225417, -1.507493, -1.282729, -1.087818, -0.981075, -0.823016, -0.912036, -0.832075},
	{-15.754005, 7.220482, 2.731196, 0.027516, -1.921565, -1.665574, -0.962624, -0.521149, -0.394428, -0.224880, -0.590567, -0.740767},
	{-14.886635, 7.522751, 2.376703, 0.229444, -2.547874, -1.230166, -0.998631, -0.764926, -0.405403, -0.151963, -0.252777, -0.677893},
	{-13.452543, 7.518717, 2.297773, 0.298150, -2.649593, -1.351853, -1.022569, -0.747487, -0.377254, -0.298008, -0.215301, -0.572234},
	{-11.834751, 7.144723, 2.640096, 0.410373, -2.799223, -1.124777, -1.500907, -0.580436, -0.200475, -0.334032, 0.068566, -0.483386},
	{-10.157672, 6.796714, 2.921605, 0.462396, -4.220464, -0.078633, -1.961881, -0.571732, -0.141882, -0.643033, 0.146373, -0.123903},
	{-6.180136, 6.039442, 2.125645, -0.868513, -3.182580, -0.632985, -1.513999, -0.355103, -0.196039, -0.635510, 0.085315, -0.124427},
	{-3.636743, 5.751954, 1.483710, -1.681069, -3.863545, 0.593872, -0.486290, -0.689209, -0.320246, -0.328275, -0.470398, -0.359179},
	{-0.832388, 5.346994, -0.217842, -1.868113, -3.705178, 0.740520, 0.215701, -0.581965, -0.599269, -0.400369, -0.319752, -0.447152},
	{0.854550, 5.241096, -2.297707, -0.906461, -3.735073, 1.383124, 0.079785, -0.597083, -0.968327, -0.035039, -0.481041, -0.512507},
	{2.911508, 4.048878, -2.872818, -0.637072, -3.076904, 1.744950, -0.201236, -0.637205, -1.201555, 0.041426, -0.318323, -0.442790},
	{3.518468, 3.647319, -2.907437, -1.130607, -2.113594, 1.726874, -0.211826, -0.853304, -1.285036, -0.015483, -0.164914, -0.469783},
	{3.964244, 2.925623, -2.301615, -1.290647, -1.473401, 1.820669, -0.834982, -0.624393, -1.187623, -0.012943, -0.390484, -0.392833},
	{2.746197, 3.157597, -2.013094, -0.804172, -1.418582, 1.829418, -1.013314, -0.710049, -1.051463, 0.098911, -0.364074, -0.577428},
	{0.474078, 4.011804, -1.451385, -0.543760, -1.343198, 1.705765, -1.264779, -0.348313, -1.076750, 0.108426, -0.094109, -0.362995},
	{-2.157344, 4.662204, -0.374911, -0.558091, -1.219016, 1.484538, -1.341950, -0.376067, -0.598382, -0.043121, 0.145904, -0.762332},
	{-6.423768, 4.398304, 0.277399, 0.747066, -0.737431, 0.945007, -0.951427, -1.216710, -0.325489, 0.262322, -0.155679, -0.561924},
	{-7.704023, 3.489149, 1.269971, 0.924836, -0.204166, 0.809042, -1.357946, -1.077550, -0.823563, 1.235458, -0.796451, -0.410074}
};

#include "model.h"

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

int main()
{
	int obs_length = 18;

	float *emis;
	emis = (float *) calloc(obs_length * STATE_NUM, sizeof(float));

	calc_emis(obs_length, test_input, 0, emis);

	int state_index;
	int obs_index;
	for (obs_index = 0; obs_index < obs_length; obs_index++) {
		for (state_index = 0; state_index < STATE_NUM; state_index++) {
			printf("%e\t", *(emis + obs_index*STATE_NUM + state_index));
		}
		printf("\n");
	}

	free(emis);

	return 0;
}
