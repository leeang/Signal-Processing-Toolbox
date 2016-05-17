#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <fract.h>
#include <complex.h>
#include <filter.h>

#define PI	3.14159265358979323846

#define WINDOW_LENGTH		512
#define WINDOW_LENGTH_HALF	256
#define FRAME_NUM			180
#define SAMPLE_LENGTH		46336
// (FRAME_NUM+1) * WINDOW_LENGTH/2

#define CHUNCK_NUM			8
#define CHUNCK_LENGTH		5792
// CHUNCK_LENGTH = SAMPLE_LENGTH / CHUNCK_NUM

#define LMS_LENGTH			128
#define LMS_LENGTH_MASK		127
#define LMS_STEP_SIZE		0.1
#define UPDATE_INTERVAL		4
#define MOD(CURRENT)		( ((CURRENT) + LMS_LENGTH) & LMS_LENGTH_MASK )
// for the case LMS_LENGTH = 128, MOD(-1) = 127, MOD(0) = 0, MOD(1) = 1

#define ENERGY_THRESHOLD	0.04

/* zero crossing count */
#define IS_ZC(X, Y)			( (X) > 0 && (Y) < 0 || (X) < 0 && (Y) > 0 )

/* FFT */
#define TWIDDLE_STRIDE		1
#define DYNAMIC_SCALING		2

/* Mel filter */
#define BANK_NUM			20
#define BANK_NUM_HALF		10
#define FEAT_NUM			13
#define BANK_GAIN_LENGTH	477

/* detection */
#define STATE_NUM			5
#define WORD_NUM			27
#define Inf					10000

#define EMIS(OBS_INDEX, STATE_INDEX)	(*(ptr_to_emis + (OBS_INDEX) * STATE_NUM + STATE_INDEX))
#define PHI(OBS_INDEX, STATE_INDEX)		(*(ptr_to_phi + (OBS_INDEX) * STATE_NUM + STATE_INDEX))

/* the coefficients of an IIR filter */
struct IIR_Coef {
	fract32 a1, a2;
	fract32 b0, b1, b2;
};

/* Global Coefficients */
struct IIR_Coef shelving_coef;
fract32 hamming_coef[WINDOW_LENGTH] = {0};
fract32 energy_threshold = 0;
/* /Global Coefficients */

/* FFT */
complex_fract32 fft_spectrum[WINDOW_LENGTH] = {0};
complex_fract32 twiddle_table[WINDOW_LENGTH/2] = {0};

/* Mel filter */
fract32 bank_gain[BANK_GAIN_LENGTH] = {0};

/* discrete cosine transform */
float dct_coef[FEAT_NUM][BANK_NUM/2] = {0.0};

const char word_string[][15] = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
	"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen",
	"nineteen", "twenty", "zero", "on", "off", "channel", "switch", "volume up", "volume down",
	"louder please"
};

/* input buffers */
section("sdram0") float cmd_noise[CHUNCK_LENGTH] = {0.0};
section("sdram0") float noise[CHUNCK_LENGTH] = {0.0};
section("sdram0") fract32 cmd_fr32[SAMPLE_LENGTH];

/* MFCC matrix */
section("L2_sram") float mfcc_matrix[FRAME_NUM][FEAT_NUM] = {0.0};

/* LMS static variables */
float weights[LMS_LENGTH] = {0.0};
float noise_buffer[LMS_LENGTH] = {0.0};
