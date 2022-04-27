#include "LPS_DCMI.h"

extern DCMI_HandleTypeDef hdcmi;
extern uint8_t cameraLineBuffer0[CAMERA_LINE_SIZE] __attribute__ ((aligned (32)));
extern uint8_t cameraLineBuffer1[CAMERA_LINE_SIZE] __attribute__ ((aligned (32)));
extern bool process_line;
extern uint32_t line_number;

uint32_t pLineData0 = (uint32_t) cameraLineBuffer0;
uint32_t pLineData1 = (uint32_t) cameraLineBuffer1;
uint32_t active_buffer = 0;

void DCMI_DMA_LineTransferCompletedCallback(DMA_HandleTypeDef *hdma)
{
	if (line_number >= CAMERA_RES_Y)
		return;

	active_buffer = !active_buffer;

	if (active_buffer == 0)
		HAL_DMA_Start_IT(hdma, (uint32_t) &(&hdcmi)->Instance->DR, pLineData0, CAMERA_LINE_SIZE/4);
	else
		HAL_DMA_Start_IT(hdma, (uint32_t) &(&hdcmi)->Instance->DR, pLineData1, CAMERA_LINE_SIZE/4);

	process_line = true;
}

void DCMI_DMA_Error(DMA_HandleTypeDef *hdma)
{

}

HAL_StatusTypeDef DCMI_Start_DMA_line(DCMI_HandleTypeDef *hdcmi, uint32_t DCMI_Mode)
{
  /* Process Locked */
  __HAL_LOCK(hdcmi);

  /* Lock the DCMI peripheral state */
  hdcmi->State = HAL_DCMI_STATE_BUSY;

  /* Enable DCMI by setting DCMIEN bit */
  __HAL_DCMI_ENABLE(hdcmi);

  /* Configure the DCMI Mode */
  hdcmi->Instance->CR &= ~(DCMI_CR_CM);
  hdcmi->Instance->CR |= (uint32_t)(DCMI_Mode);

  /* Set DMA callbacks */
  hdcmi->DMA_Handle->XferCpltCallback  = DCMI_DMA_LineTransferCompletedCallback;
  hdcmi->DMA_Handle->XferErrorCallback = DCMI_DMA_Error;

  /* Enable the DMA Stream */
  HAL_DMA_Start_IT(hdcmi->DMA_Handle, (uint32_t)&hdcmi->Instance->DR, pLineData0, CAMERA_LINE_SIZE/4);

  /* Enable Capture */
  hdcmi->Instance->CR |= DCMI_CR_CAPTURE;

  /* Release Lock */
  __HAL_UNLOCK(hdcmi);

  /* Return function status */
  return HAL_OK;
}

