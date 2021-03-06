#ifndef INC_LPS_COMMUNICATION_H_
#define INC_LPS_COMMUNICATION_H_

#include <stdint.h>
#include "printf.h"

void printDataToMatlab(uint32_t *h_values, uint32_t h_size, uint32_t *v_values, uint32_t v_size);
void printCOGToMatlab(double cog_h, double cog_v);

#endif /* INC_LPS_COMMUNICATION_H_ */
