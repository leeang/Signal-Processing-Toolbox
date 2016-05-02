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

#define ENERGY_THRSH		0.04

/* FIR filter */
#define BUFFER_SIZE			2
	//define the buffer size
#define INDEX(CURRENT)		( ((CURRENT) + BUFFER_SIZE) % BUFFER_SIZE )
	// if an index is negative, a specified position from the end of the array will be returned.
	// e.g. given an array x[8], x[INDEX(-1)] and x[INDEX(7)] both refer to x[7].
/* /FIR filter */

/* zero crossing count */
#define IS_ZC(X, Y)		( (X) > 0 && (Y) < 0 || (X) < 0 && (Y) > 0 )

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
/* /Global Coefficients */

/* FFT */
complex_fract32 fft_spectrum[WINDOW_LENGTH] = {0};
complex_fract32 twiddle_table[WINDOW_LENGTH/2] = {0};

/* Mel filter */
fract32 bank_gain[BANK_GAIN_LENGTH] = {0};

/* discrete cosine transform */
float dct_coef[FEAT_NUM][BANK_NUM/2] = {0.0};

const char word_string[][12] = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
	"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen",
	"nineteen", "twenty", "zero", "on", "off", "channel", "switch", "volume up", "volume down"
};

section("sdram0") fract32 input_fr[SAMPLE_LENGTH];
section("L2_sram") float mfcc_matrix[FRAME_NUM][FEAT_NUM] = {0.0};
