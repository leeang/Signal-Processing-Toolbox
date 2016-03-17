#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define TOTAL_LENGTH	512
#define WINDOW_LENGTH	128
#define FRAME_NUM		(TOTAL_LENGTH / WINDOW_LENGTH * 2 - 1)

int test_input[TOTAL_LENGTH];

void frame(int input[], int output[FRAME_NUM][WINDOW_LENGTH])
{
	int frame_num, index;

	int offset = 0;
	for (frame_num = 0; frame_num < FRAME_NUM; frame_num++) {
		for (index = 0; index < WINDOW_LENGTH; index++) {
			output[frame_num][index] = input[index+offset];
		}
		offset += WINDOW_LENGTH/2;
	}
}

int main() {
	int index;
	for (index = 0; index < TOTAL_LENGTH; index++) {
		test_input[index] = index+1;
	}

	int after_frame[FRAME_NUM][WINDOW_LENGTH];
	frame(test_input, after_frame);

	int frame_num;
	for (frame_num = 0; frame_num < FRAME_NUM; frame_num++) {
		for (index = 0; index < WINDOW_LENGTH; index++) {
			printf("%d\t", after_frame[frame_num][index]);
		}
		printf("\n");
	}

	return 0;
}
