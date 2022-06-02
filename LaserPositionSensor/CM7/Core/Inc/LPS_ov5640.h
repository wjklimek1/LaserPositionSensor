#ifndef INC_LPS_OV5640_H_
#define INC_LPS_OV5640_H_

#include <stdint.h>

#include "LPS_ov5640_defines.h"

void ov5640_read_reg(uint16_t reg, uint8_t *data, uint16_t data_size);
void ov5640_write_reg(uint16_t reg, uint8_t *data, uint16_t data_size);

int32_t ov5640_init();
int32_t ov5640_readID(uint32_t *id);
int32_t ov5640_setResolution(uint32_t resolution);
int32_t ov5640_SetPixelFormat(uint32_t PixelFormat);
int32_t ov5640_SetPolarities(uint32_t PclkPol, uint32_t HrefPol, uint32_t VsyncPol);
int32_t ov5640_enableTestMode();
int32_t ov5640_disableAutoExposure();
int32_t ov5640_setManualExposure(uint16_t exposure, uint8_t gain);


#endif /* INC_LPS_OV5640_H_ */
