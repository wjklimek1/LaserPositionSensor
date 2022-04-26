#ifndef INC_LPS_DCMI_H_
#define INC_LPS_DCMI_H_

#include <stdbool.h>
#include "dcmi.h"

#define CAMERA_RES_X              (640)
#define CAMERA_RES_Y              (480)
#define CAMERA_NUM_PIXELS         (CAMERA_RES_X * CAMERA_RES_Y)
#define CAMERA_BITS_PER_PX        16U
#define CAMERA_LINE_SIZE          (CAMERA_RES_X * CAMERA_BITS_PER_PX / 8)
#define CAMERA_BUFF_SIZE          (CAMERA_NUM_PIXELS * CAMERA_BITS_PER_PX / 8)

void DCMI_DMALineXferCplt(DMA_HandleTypeDef *hdma);
void DCMI_DMAError(DMA_HandleTypeDef *hdma);

HAL_StatusTypeDef DCMI_Start_DMA_line(DCMI_HandleTypeDef *hdcmi, uint32_t DCMI_Mode);

#endif /* INC_LPS_DCMI_H_ */
