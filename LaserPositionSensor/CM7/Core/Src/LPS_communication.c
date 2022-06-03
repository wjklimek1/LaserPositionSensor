#ifndef SRC_LPS_COMMUNICATION_C_
#define SRC_LPS_COMMUNICATION_C_

#include "LPS_communication.h"

void printDataToMatlab(uint32_t *h_values, uint32_t h_size, uint32_t *v_values, uint32_t v_size)
{
	printf("data_horizontal = [");
	for(uint32_t i = 0; i < h_size - 1; i++)
	{
		printf("%d,", h_values[i]);
	}
	printf("%d]", h_values[h_size - 1]);
	printf("; ");

	printf("data_vertical = [");
	for(uint32_t i = 0; i < v_size - 1; i++)
	{
		printf("%d,", v_values[i]);
	}
	printf("%d]", v_values[v_size - 1]);
	printf("; ");

	printf("\n");
}

#endif /* SRC_LPS_COMMUNICATION_C_ */
