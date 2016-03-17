#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PI
#define PI	3.14159265358979323846
#endif

/* sample type */
typedef double smp_type;

#define IS_ZC(X, Y)	( (X) > 0 && (Y) < 0 || (X) < 0 && (Y) > 0 ? 1 : 0 )

int zc_count(smp_type data[], int arr_length)
{
	int zc_num = 0;
	for (int index = 1; index < arr_length; index++) {
		int res = IS_ZC(data[index], data[index-1]);
		zc_num += res;
	}
	return zc_num;
}

int main() {
	int window_length = 360;

	int zc_num = zc_count(test_input, window_length);

	printf("%d\n", zc_num);

	return 0;
}
