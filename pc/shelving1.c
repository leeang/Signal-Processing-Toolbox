#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PI
#define PI	3.14159265358979323846
#endif

/* sample type */
typedef double smp_type;

/* this holds the data required to update samples through a filter */
struct Coef {
	smp_type a0, a1, a2;
	smp_type b0, b1, b2;
	smp_type x1, x2, y1, y2;
};

struct Coef shelving(smp_type dbGain, smp_type fc, smp_type fs, smp_type Q) {
	struct Coef coefficients;

	smp_type K = tan( (PI * fc)/fs );
	smp_type K_squre = K * K;
	smp_type V0 = pow(10, dbGain/20);
	smp_type root2 = 1/Q;

	coefficients.b0 = (V0 + root2 * sqrt(V0) * K + K_squre) / (1 + root2 * K + K_squre);
	coefficients.b1 = (2 * (K_squre - V0) ) / (1 + root2 * K + K_squre);
	coefficients.b2 = (V0 - root2 * sqrt(V0)*K + K_squre) / (1 + root2 * K + K_squre);
	coefficients.a0 = 1;
	coefficients.a1 = (2 * (K_squre - 1) ) / (1 + root2 * K + K_squre);
	coefficients.a2 = (1 - root2 * K + K_squre) / (1 + root2 * K + K_squre);

    /* zero initial samples */
    coefficients.x1 = coefficients.x2 = 0;
    coefficients.y1 = coefficients.y2 = 0;

    return coefficients;
}

smp_type filter(smp_type input, struct Coef *coefficients)
{
	smp_type output;

	/* compute output */
	output = (coefficients->b0 * input + coefficients->b1 * coefficients->x1 + coefficients->b2 * coefficients->x2) / coefficients->a0 - coefficients->a1 * coefficients->y1 - coefficients->a2 * coefficients->y2;

	/* shift x1 to x2, input to x1 */
	coefficients->x2 = coefficients->x1;
	coefficients->x1 = input;

	/* shift y1 to y2, output to y1 */
	coefficients->y2 = coefficients->y1;
	coefficients->y1 = output;

	return output;
}

float input[] = { 0.000000, 0.311155, 0.604528, 0.863541, 1.073937, 
		1.224745, 1.309017, 1.324294, 1.272762, 1.161097, 
		1.000000, 0.803458, 0.587785, 0.370501, 0.169131, 
		0.000000, -0.122881, -0.188780, -0.190983, -0.127255, 
		0.000000, 0.183900, 0.413545, 0.674761, 0.951057, 
		1.224745, 1.478148, 1.694795, 1.860547, 1.964555, 
		2.000000, 1.964555, 1.860547, 1.694795, 1.478148, 
		1.224745, 0.951057, 0.674761, 0.413545, 0.183900, 
		-0.000000, -0.127255, -0.190983, -0.188780, -0.122881, 
		-0.000000, 0.169131, 0.370501, 0.587785, 0.803458, 
		1.000000, 1.161097, 1.272762, 1.324294, 1.309017, 
		1.224745, 1.073937, 0.863541, 0.604528, 0.311155, 
		0.000000, -0.311155, -0.604528, -0.863541, -1.073937, 
		-1.224745, -1.309017, -1.324294, -1.272762, -1.161097, 
		-1.000000, -0.803458, -0.587785, -0.370501, -0.169131, 
		0.000000, 0.122881, 0.188780, 0.190983, 0.127255, 
		-0.000000, -0.183900, -0.413545, -0.674761, -0.951057, 
		-1.224745, -1.478148, -1.694795, -1.860547, -1.964555, 
		-2.000000, -1.964555, -1.860547, -1.694795, -1.478148, 
		-1.224745, -0.951057, -0.674761, -0.413545, -0.183900, 
		-0.000000, 0.127255, 0.190983, 0.188780, 0.122881, 
		0.000000, -0.169131, -0.370501, -0.587785, -0.803458, 
		-1.000000, -1.161097, -1.272762, -1.324294, -1.309017, 
		-1.224745, -1.073937, -0.863541, -0.604528, -0.311155, 
		-0.000000, 0.311155, 0.604528, 0.863541, 1.073937, 
		1.224745, 1.309017, 1.324294, 1.272762, 1.161097, 
		1.000000, 0.803458, 0.587785, 0.370501, 0.169131, 
		0.000000, -0.122881, -0.188780, -0.190983, -0.127255, 
		0.000000, 0.183900, 0.413545, 0.674761, 0.951057, 
		1.224745, 1.478148, 1.694795, 1.860547, 1.964555, 
		2.000000, 1.964555, 1.860547, 1.694795, 1.478148, 
		1.224745, 0.951057, 0.674761, 0.413545, 0.183900, 
		-0.000000, -0.127255, -0.190983, -0.188780, -0.122881, 
		-0.000000, 0.169131, 0.370501, 0.587785, 0.803458, 
		1.000000, 1.161097, 1.272762, 1.324294, 1.309017, 
		1.224745, 1.073937, 0.863541, 0.604528, 0.311155, 
		0.000000, -0.311155, -0.604528, -0.863541, -1.073937, 
		-1.224745, -1.309017, -1.324294, -1.272762, -1.161097, 
		-1.000000, -0.803458, -0.587785, -0.370501, -0.169131, 
		-0.000000, 0.122881, 0.188780, 0.190983, 0.127255, 
		0.000000, -0.183900, -0.413545, -0.674761, -0.951057, 
		-1.224745, -1.478148, -1.694795, -1.860547, -1.964555, 
		-2.000000, -1.964555, -1.860547, -1.694795, -1.478148, 
		-1.224745, -0.951057, -0.674761, -0.413545, -0.183900, 
		-0.000000, 0.127255, 0.190983, 0.188780, 0.122881, 
		0.000000, -0.169131, -0.370501, -0.587785, -0.803458, 
		-1.000000, -1.161097, -1.272762, -1.324294, -1.309017, 
		-1.224745, -1.073937, -0.863541, -0.604528, -0.311155, 
		-0.000000, 0.311155, 0.604528, 0.863541, 1.073937, 
		1.224745, 1.309017, 1.324294, 1.272762, 1.161097, 
		1.000000, 0.803458, 0.587785, 0.370501, 0.169131, 
		0.000000, -0.122881, -0.188780, -0.190983, -0.127255, 
		-0.000000, 0.183900, 0.413545, 0.674761, 0.951057, 
		1.224745, 1.478148, 1.694795, 1.860547, 1.964555, 
		2.000000, 1.964555, 1.860547, 1.694795, 1.478148, 
		1.224745, 0.951057, 0.674761, 0.413545, 0.183900, 
		0.000000, -0.127255, -0.190983, -0.188780, -0.122881, 
		-0.000000, 0.169131, 0.370501, 0.587785, 0.803458, 
		1.000000, 1.161097, 1.272762, 1.324294, 1.309017, 
		1.224745, 1.073937, 0.863541, 0.604528, 0.311155, 
		-0.000000, -0.311155, -0.604528, -0.863541, -1.073937, 
		-1.224745, -1.309017, -1.324294, -1.272762, -1.161097, 
		-1.000000, -0.803458, -0.587785, -0.370501, -0.169131, 
		-0.000000, 0.122881, 0.188780, 0.190983, 0.127255, 
		-0.000000, -0.183900, -0.413545, -0.674761, -0.951057, 
		-1.224745, -1.478148, -1.694795, -1.860547, -1.964555, 
		-2.000000, -1.964555, -1.860547, -1.694795, -1.478148, 
		-1.224745, -0.951057, -0.674761, -0.413545, -0.183900, 
		-0.000000, 0.127255, 0.190983, 0.188780, 0.122881, 
		0.000000, -0.169131, -0.370501, -0.587785, -0.803458, 
		-1.000000, -1.161097, -1.272762, -1.324294, -1.309017, 
		-1.224745, -1.073937, -0.863541, -0.604528, -0.311155, 
		-0.000000, 0.311155, 0.604528, 0.863541, 1.073937, 
		1.224745, 1.309017, 1.324294, 1.272762, 1.161097, 
		1.000000, 0.803458, 0.587785, 0.370501, 0.169131, 
		0.000000, -0.122881, -0.188780, -0.190983, -0.127255, 
		0.000000, 0.183900, 0.413545, 0.674761, 0.951057, 
		1.224745, 1.478148, 1.694795, 1.860547, 1.964555, 
		2.000000, 1.964555, 1.860547, 1.694795, 1.478148, 
		1.224745, 0.951057, 0.674761, 0.413545, 0.183900, 
		0.000000, -0.127255, -0.190983, -0.188780, -0.122881, 
		-0.000000, 0.169131, 0.370501, 0.587785, 0.803458, 
		1.000000, 1.161097, 1.272762, 1.324294, 1.309017, 
		1.224745, 1.073937, 0.863541, 0.604528, 0.311155, 
		0.000000, -0.311155, -0.604528, -0.863541, -1.073937, 
		-1.224745, -1.309017, -1.324294, -1.272762, -1.161097, 
		-1.000000, -0.803458, -0.587785, -0.370501, -0.169131, 
		-0.000000, 0.122881, 0.188780, 0.190983, 0.127255, 
		0.000000, -0.183900, -0.413545, -0.674761, -0.951057, 
		-1.224745, -1.478148, -1.694795, -1.860547, -1.964555, 
		-2.000000, -1.964555, -1.860547, -1.694795, -1.478148, 
		-1.224745, -0.951057, -0.674761, -0.413545, -0.183900, 
		-0.000000, 0.127255, 0.190983, 0.188780, 0.122881, 
		0.000000, -0.169131, -0.370501, -0.587785, -0.803458, 
		-1.000000, -1.161097, -1.272762, -1.324294, -1.309017, 
		-1.224745, -1.073937, -0.863541, -0.604528, -0.311155, 
		-0.000000, 0.311155, 0.604528, 0.863541, 1.073937, 
		1.224745, 1.309017, 1.324294, 1.272762, 1.161097, 
		1.000000, 0.803458, 0.587785, 0.370501, 0.169131, 
		0.000000, -0.122881, -0.188780, -0.190983, -0.127255, 
		-0.000000, 0.183900, 0.413545, 0.674761, 0.951057, 
		1.224745, 1.478148, 1.694795, 1.860547, 1.964555, 
		2.000000, 1.964555, 1.860547, 1.694795, 1.478148, 
		1.224745, 0.951057, 0.674761, 0.413545, 0.183900, 
		0.000000, -0.127255, -0.190983, -0.188780, -0.122881, 
		-0.000000, 0.169131, 0.370501, 0.587785, 0.803458, 
		1.000000, 1.161097, 1.272762, 1.324294, 1.309017, 
		1.224745, 1.073937, 0.863541, 0.604528, 0.311155, 
		0.000000, -0.311155, -0.604528, -0.863541, -1.073937, 
		-1.224745, -1.309017, -1.324294, -1.272762, -1.161097, 
		-1.000000, -0.803458, -0.587785, -0.370501, -0.169131, 
		-0.000000, 0.122881, 0.188780, 0.190983, 0.127255, 
		-0.000000, -0.183900, -0.413545, -0.674761, -0.951057, 
		-1.224745, -1.478148, -1.694795, -1.860547, -1.964555, 
		-2.000000, -1.964555, -1.860547, -1.694795, -1.478148, 
		-1.224745, -0.951057, -0.674761, -0.413545, -0.183900, 
		-0.000000, 0.127255, 0.190983, 0.188780, 0.122881, 
		0.000000, -0.169131, -0.370501, -0.587785, -0.803458, 
		-1.000000, -1.161097, -1.272762, -1.324294, -1.309017, 
		-1.224745, -1.073937, -0.863541, -0.604528, -0.311155 };

int main() {
	struct Coef coefficients = shelving(6, 1000, 16E3, 0.9);
	// printf("%.4f\n", coefficients.a0);
	// printf("%.4f\n", coefficients.a1);
	// printf("%.4f\n", coefficients.a2);
	// printf("%.4f\n", coefficients.b0);
	// printf("%.4f\n", coefficients.b1);
	// printf("%.4f\n", coefficients.b2);

    for (int i = 0; i < 360; ++i) {
        smp_type result = filter(input[i], &coefficients);
        printf("%.4f\t", result);
    }
	return 0;
}
