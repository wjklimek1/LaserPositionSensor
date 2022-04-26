#include "LPS_DCMI.h"

extern DCMI_HandleTypeDef hdcmi;
extern uint8_t cameraLineBuffer[CAMERA_LINE_SIZE] __attribute__ ((aligned (32)));
extern int vsync;
extern int hsync;
extern int dma_line;

void DCMI_DMALineXferCplt(DMA_HandleTypeDef *hdma)
{
	dma_line++;
}

void DCMI_DMAError(DMA_HandleTypeDef *hdma)
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
  hdcmi->DMA_Handle->XferCpltCallback  = DCMI_DMALineXferCplt;
  hdcmi->DMA_Handle->XferErrorCallback = DCMI_DMAError;
  hdcmi->DMA_Handle->XferAbortCallback = NULL;

  hdcmi->XferCount = 0;
  hdcmi->XferTransferNumber = 0;
  hdcmi->XferSize = CAMERA_LINE_SIZE / 4U;
  hdcmi->pBuffPtr = (uint32_t) cameraLineBuffer;

  /* Enable the DMA Stream */
  uint32_t pLineData = (uint32_t) cameraLineBuffer;
  HAL_DMA_Start_IT(hdcmi->DMA_Handle, (uint32_t)&hdcmi->Instance->DR, pLineData, hdcmi->XferSize);

  /* Enable Capture */
  hdcmi->Instance->CR |= DCMI_CR_CAPTURE;

  /* Release Lock */
  __HAL_UNLOCK(hdcmi);

  /* Return function status */
  return HAL_OK;
}

void emptyLineBuffer()
{
	for (uint32_t i = 0; i < CAMERA_LINE_SIZE; i++)
	{
		cameraLineBuffer[i] = 0;
	}
}
