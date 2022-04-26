#include "LPS_ov5640.h"
#include "LPS_ov5640_defines.h"

#include "i2c.h"

void ov5640_read_reg(uint16_t reg, uint8_t *data, uint16_t data_size)
{
	HAL_I2C_Mem_Read(&hi2c4, OV5640_I2C_ADDR, reg, 2, data, data_size, 200);
}

void ov5640_write_reg(uint16_t reg, uint8_t *data, uint16_t data_size)
{
	HAL_I2C_Mem_Write(&hi2c4, OV5640_I2C_ADDR, reg, 2, data, data_size, 200);
}

int32_t ov5640_init()
{
	uint8_t tmp;
	uint32_t index;
	int32_t ret = OV5640_OK;

	/* Initialization sequence for OV5640 */
	static const uint16_t OV5640_Common[][2] =
	{
	  {OV5640_SCCB_SYSTEM_CTRL1, 0x11},
	  {OV5640_SYSTEM_CTROL0, 0x82},
	  {OV5640_SCCB_SYSTEM_CTRL1, 0x03},
	  {OV5640_PAD_OUTPUT_ENABLE01, 0xFF},
	  {OV5640_PAD_OUTPUT_ENABLE02, 0xf3},
	  {OV5640_SC_PLL_CONTRL0, 0x18},
	  {OV5640_SYSTEM_CTROL0, 0x02},
	  {OV5640_SC_PLL_CONTRL1, 0x41},
	  {OV5640_SC_PLL_CONTRL2, 0x30},
	  {OV5640_SC_PLL_CONTRL3, 0x13},
	  {OV5640_SYSTEM_ROOT_DIVIDER, 0x01},
	  {0x3630, 0x36},
	  {0x3631, 0x0e},
	  {0x3632, 0xe2},
	  {0x3633, 0x12},
	  {0x3621, 0xe0},
	  {0x3704, 0xa0},
	  {0x3703, 0x5a},
	  {0x3715, 0x78},
	  {0x3717, 0x01},
	  {0x370b, 0x60},
	  {0x3705, 0x1a},
	  {0x3905, 0x02},
	  {0x3906, 0x10},
	  {0x3901, 0x0a},
	  {0x3731, 0x12},
	  {0x3600, 0x08},
	  {0x3601, 0x33},
	  {0x302d, 0x60},
	  {0x3620, 0x52},
	  {0x371b, 0x20},
	  {0x471c, 0x50},
	  {OV5640_AEC_CTRL13, 0x43},
	  {OV5640_AEC_GAIN_CEILING_HIGH, 0x00},
	  {OV5640_AEC_GAIN_CEILING_LOW, 0xf8},
	  {0x3635, 0x13},
	  {0x3636, 0x03},
	  {0x3634, 0x40},
	  {0x3622, 0x01},
	  {OV5640_5060HZ_CTRL01, 0x34},
	  {OV5640_5060HZ_CTRL04, 0x28},
	  {OV5640_5060HZ_CTRL05, 0x98},
	  {OV5640_LIGHTMETER1_TH_HIGH, 0x00},
	  {OV5640_LIGHTMETER1_TH_LOW, 0x00},
	  {OV5640_LIGHTMETER2_TH_HIGH, 0x01},
	  {OV5640_LIGHTMETER2_TH_LOW, 0x2c},
	  {OV5640_SAMPLE_NUMBER_HIGH, 0x9c},
	  {OV5640_SAMPLE_NUMBER_LOW, 0x40},
	  {OV5640_TIMING_TC_REG20, 0x06},
	  {OV5640_TIMING_TC_REG21, 0x00},
	  {OV5640_TIMING_X_INC, 0x31},
	  {OV5640_TIMING_Y_INC, 0x31},
	  {OV5640_TIMING_HS_HIGH, 0x00},
	  {OV5640_TIMING_HS_LOW, 0x00},
	  {OV5640_TIMING_VS_HIGH, 0x00},
	  {OV5640_TIMING_VS_LOW, 0x04},
	  {OV5640_TIMING_HW_HIGH, 0x0a},
	  {OV5640_TIMING_HW_LOW, 0x3f},
	  {OV5640_TIMING_VH_HIGH, 0x07},
	  {OV5640_TIMING_VH_LOW, 0x9b},
	  {OV5640_TIMING_DVPHO_HIGH, 0x03},
	  {OV5640_TIMING_DVPHO_LOW, 0x20},
	  {OV5640_TIMING_DVPVO_HIGH, 0x02},
	  {OV5640_TIMING_DVPVO_LOW, 0x58},
	  {OV5640_TIMING_HTS_HIGH, 0x06},
	  {OV5640_TIMING_HTS_LOW, 0x40},
	  {OV5640_TIMING_VTS_HIGH, 0x03},
	  {OV5640_TIMING_VTS_LOW, 0xe8},
	  {OV5640_TIMING_HOFFSET_HIGH, 0x00},
	  {OV5640_TIMING_HOFFSET_LOW, 0x10},
	  {OV5640_TIMING_VOFFSET_HIGH, 0x00},
	  {OV5640_TIMING_VOFFSET_LOW, 0x06},
	  {0x3618, 0x00},
	  {0x3612, 0x29},
	  {0x3708, 0x64},
	  {0x3709, 0x52},
	  {0x370c, 0x03},
	  {OV5640_AEC_CTRL02, 0x03},
	  {OV5640_AEC_CTRL03, 0xd8},
	  {OV5640_AEC_B50_STEP_HIGH, 0x01},
	  {OV5640_AEC_B50_STEP_LOW, 0x27},
	  {OV5640_AEC_B60_STEP_HIGH, 0x00},
	  {OV5640_AEC_B60_STEP_LOW, 0xf6},
	  {OV5640_AEC_CTRL0E, 0x03},
	  {OV5640_AEC_CTRL0D, 0x04},
	  {OV5640_AEC_MAX_EXPO_HIGH, 0x03},
	  {OV5640_AEC_MAX_EXPO_LOW, 0xd8},
	  {OV5640_BLC_CTRL01, 0x02},
	  {OV5640_BLC_CTRL04, 0x02},
	  {OV5640_SYSREM_RESET00, 0x00},
	  {OV5640_SYSREM_RESET02, 0x1c},
	  {OV5640_CLOCK_ENABLE00, 0xff},
	  {OV5640_CLOCK_ENABLE02, 0xc3},
	  {OV5640_MIPI_CONTROL00, 0x58},
	  {0x302e, 0x00},
	  {OV5640_POLARITY_CTRL, 0x22},
	  {OV5640_FORMAT_CTRL00, 0x6F},
	  {OV5640_FORMAT_MUX_CTRL, 0x01},
	  {OV5640_JPG_MODE_SELECT, 0x03},
	  {OV5640_JPEG_CTRL07, 0x04},
	  {0x440e, 0x00},
	  {0x460b, 0x35},
	  {0x460c, 0x23},
	  {OV5640_PCLK_PERIOD, 0x22},
	  {0x3824, 0x02},
	  {OV5640_ISP_CONTROL00, 0xa7},
	  {OV5640_ISP_CONTROL01, 0xa3},
	  {OV5640_AWB_CTRL00, 0xff},
	  {OV5640_AWB_CTRL01, 0xf2},
	  {OV5640_AWB_CTRL02, 0x00},
	  {OV5640_AWB_CTRL03, 0x14},
	  {OV5640_AWB_CTRL04, 0x25},
	  {OV5640_AWB_CTRL05, 0x24},
	  {OV5640_AWB_CTRL06, 0x09},
	  {OV5640_AWB_CTRL07, 0x09},
	  {OV5640_AWB_CTRL08, 0x09},
	  {OV5640_AWB_CTRL09, 0x75},
	  {OV5640_AWB_CTRL10, 0x54},
	  {OV5640_AWB_CTRL11, 0xe0},
	  {OV5640_AWB_CTRL12, 0xb2},
	  {OV5640_AWB_CTRL13, 0x42},
	  {OV5640_AWB_CTRL14, 0x3d},
	  {OV5640_AWB_CTRL15, 0x56},
	  {OV5640_AWB_CTRL16, 0x46},
	  {OV5640_AWB_CTRL17, 0xf8},
	  {OV5640_AWB_CTRL18, 0x04},
	  {OV5640_AWB_CTRL19, 0x70},
	  {OV5640_AWB_CTRL20, 0xf0},
	  {OV5640_AWB_CTRL21, 0xf0},
	  {OV5640_AWB_CTRL22, 0x03},
	  {OV5640_AWB_CTRL23, 0x01},
	  {OV5640_AWB_CTRL24, 0x04},
	  {OV5640_AWB_CTRL25, 0x12},
	  {OV5640_AWB_CTRL26, 0x04},
	  {OV5640_AWB_CTRL27, 0x00},
	  {OV5640_AWB_CTRL28, 0x06},
	  {OV5640_AWB_CTRL29, 0x82},
	  {OV5640_AWB_CTRL30, 0x38},
	  {OV5640_CMX1, 0x1e},
	  {OV5640_CMX2, 0x5b},
	  {OV5640_CMX3, 0x08},
	  {OV5640_CMX4, 0x0a},
	  {OV5640_CMX5, 0x7e},
	  {OV5640_CMX6, 0x88},
	  {OV5640_CMX7, 0x7c},
	  {OV5640_CMX8, 0x6c},
	  {OV5640_CMX9, 0x10},
	  {OV5640_CMXSIGN_HIGH, 0x01},
	  {OV5640_CMXSIGN_LOW, 0x98},
	  {OV5640_CIP_SHARPENMT_TH1, 0x08},
	  {OV5640_CIP_SHARPENMT_TH2, 0x30},
	  {OV5640_CIP_SHARPENMT_OFFSET1, 0x10},
	  {OV5640_CIP_SHARPENMT_OFFSET2, 0x00},
	  {OV5640_CIP_DNS_TH1, 0x08},
	  {OV5640_CIP_DNS_TH2, 0x30},
	  {OV5640_CIP_DNS_OFFSET1, 0x08},
	  {OV5640_CIP_DNS_OFFSET2, 0x16},
	  {OV5640_CIP_CTRL, 0x08},
	  {OV5640_CIP_SHARPENTH_TH1, 0x30},
	  {OV5640_CIP_SHARPENTH_TH2, 0x04},
	  {OV5640_CIP_SHARPENTH_OFFSET1, 0x06},
	  {OV5640_GAMMA_CTRL00, 0x01},
	  {OV5640_GAMMA_YST00, 0x08},
	  {OV5640_GAMMA_YST01, 0x14},
	  {OV5640_GAMMA_YST02, 0x28},
	  {OV5640_GAMMA_YST03, 0x51},
	  {OV5640_GAMMA_YST04, 0x65},
	  {OV5640_GAMMA_YST05, 0x71},
	  {OV5640_GAMMA_YST06, 0x7d},
	  {OV5640_GAMMA_YST07, 0x87},
	  {OV5640_GAMMA_YST08, 0x91},
	  {OV5640_GAMMA_YST09, 0x9a},
	  {OV5640_GAMMA_YST0A, 0xaa},
	  {OV5640_GAMMA_YST0B, 0xb8},
	  {OV5640_GAMMA_YST0C, 0xcd},
	  {OV5640_GAMMA_YST0D, 0xdd},
	  {OV5640_GAMMA_YST0E, 0xea},
	  {OV5640_GAMMA_YST0F, 0x1d},
	  {OV5640_SDE_CTRL0, 0x02},
	  {OV5640_SDE_CTRL3, 0x40},
	  {OV5640_SDE_CTRL4, 0x10},
	  {OV5640_SDE_CTRL9, 0x10},
	  {OV5640_SDE_CTRL10, 0x00},
	  {OV5640_SDE_CTRL11, 0xf8},
	  {OV5640_GMTRX00, 0x23},
	  {OV5640_GMTRX01, 0x14},
	  {OV5640_GMTRX02, 0x0f},
	  {OV5640_GMTRX03, 0x0f},
	  {OV5640_GMTRX04, 0x12},
	  {OV5640_GMTRX05, 0x26},
	  {OV5640_GMTRX10, 0x0c},
	  {OV5640_GMTRX11, 0x08},
	  {OV5640_GMTRX12, 0x05},
	  {OV5640_GMTRX13, 0x05},
	  {OV5640_GMTRX14, 0x08},
	  {OV5640_GMTRX15, 0x0d},
	  {OV5640_GMTRX20, 0x08},
	  {OV5640_GMTRX21, 0x03},
	  {OV5640_GMTRX22, 0x00},
	  {OV5640_GMTRX23, 0x00},
	  {OV5640_GMTRX24, 0x03},
	  {OV5640_GMTRX25, 0x09},
	  {OV5640_GMTRX30, 0x07},
	  {OV5640_GMTRX31, 0x03},
	  {OV5640_GMTRX32, 0x00},
	  {OV5640_GMTRX33, 0x01},
	  {OV5640_GMTRX34, 0x03},
	  {OV5640_GMTRX35, 0x08},
	  {OV5640_GMTRX40, 0x0d},
	  {OV5640_GMTRX41, 0x08},
	  {OV5640_GMTRX42, 0x05},
	  {OV5640_GMTRX43, 0x06},
	  {OV5640_GMTRX44, 0x08},
	  {OV5640_GMTRX45, 0x0e},
	  {OV5640_GMTRX50, 0x29},
	  {OV5640_GMTRX51, 0x17},
	  {OV5640_GMTRX52, 0x11},
	  {OV5640_GMTRX53, 0x11},
	  {OV5640_GMTRX54, 0x15},
	  {OV5640_GMTRX55, 0x28},
	  {OV5640_BRMATRX00, 0x46},
	  {OV5640_BRMATRX01, 0x26},
	  {OV5640_BRMATRX02, 0x08},
	  {OV5640_BRMATRX03, 0x26},
	  {OV5640_BRMATRX04, 0x64},
	  {OV5640_BRMATRX05, 0x26},
	  {OV5640_BRMATRX06, 0x24},
	  {OV5640_BRMATRX07, 0x22},
	  {OV5640_BRMATRX08, 0x24},
	  {OV5640_BRMATRX09, 0x24},
	  {OV5640_BRMATRX20, 0x06},
	  {OV5640_BRMATRX21, 0x22},
	  {OV5640_BRMATRX22, 0x40},
	  {OV5640_BRMATRX23, 0x42},
	  {OV5640_BRMATRX24, 0x24},
	  {OV5640_BRMATRX30, 0x26},
	  {OV5640_BRMATRX31, 0x24},
	  {OV5640_BRMATRX32, 0x22},
	  {OV5640_BRMATRX33, 0x22},
	  {OV5640_BRMATRX34, 0x26},
	  {OV5640_BRMATRX40, 0x44},
	  {OV5640_BRMATRX41, 0x24},
	  {OV5640_BRMATRX42, 0x26},
	  {OV5640_BRMATRX43, 0x28},
	  {OV5640_BRMATRX44, 0x42},
	  {OV5640_LENC_BR_OFFSET, 0xce},
	  {0x5025, 0x00},
	  {OV5640_AEC_CTRL0F, 0x30},
	  {OV5640_AEC_CTRL10, 0x28},
	  {OV5640_AEC_CTRL1B, 0x30},
	  {OV5640_AEC_CTRL1E, 0x26},
	  {OV5640_AEC_CTRL11, 0x60},
	  {OV5640_AEC_CTRL1F, 0x14},
	  {OV5640_SYSTEM_CTROL0, 0x02},
	};

	/* reset camera */
	tmp = 0x80;
	ov5640_write_reg(OV5640_SYSTEM_CTROL0, &tmp, 1);
	HAL_Delay(50);

    /* Set common parameters for all resolutions */
	for (index = 0; index < (sizeof(OV5640_Common) / 4U) ; index++)
	{
		tmp = (uint8_t)OV5640_Common[index][1];
	    ov5640_write_reg(OV5640_Common[index][0], &tmp, 1);
	}

	/* Set PixelClock, Href and VSync Polarity */
	ov5640_SetPolarities(OV5640_POLARITY_PCLK_HIGH, OV5640_POLARITY_HREF_HIGH, OV5640_POLARITY_VSYNC_HIGH);
	return ret;
}

int32_t ov5640_readID(uint32_t *id)
{
	uint8_t tmp1;
	uint8_t tmp2;
	ov5640_read_reg(OV5640_CHIP_ID_HIGH_BYTE, &tmp1, 1);
	ov5640_read_reg(OV5640_CHIP_ID_LOW_BYTE,  &tmp2, 1);
	*id = (tmp1 << 8) | tmp2;
	return OV5640_OK;
}

int32_t ov5640_setResolution(uint32_t resolution)
{
	uint32_t index;
	uint8_t tmp;

	/* Initialization sequence for WVGA resolution (800x480)*/
	static const uint16_t OV5640_WVGA[][2] =
	{
	  {OV5640_TIMING_DVPHO_HIGH, 0x03},
	  {OV5640_TIMING_DVPHO_LOW, 0x20},
	  {OV5640_TIMING_DVPVO_HIGH, 0x01},
	  {OV5640_TIMING_DVPVO_LOW, 0xE0},
	};

	/* Initialization sequence for VGA resolution (640x480)*/
	static const uint16_t OV5640_VGA[][2] =
	{
	  {OV5640_TIMING_DVPHO_HIGH, 0x02},
	  {OV5640_TIMING_DVPHO_LOW, 0x80},
	  {OV5640_TIMING_DVPVO_HIGH, 0x01},
	  {OV5640_TIMING_DVPVO_LOW, 0xE0},
	};

	/* Initialization sequence for 480x272 resolution */
	static const uint16_t OV5640_480x272[][2] =
	{
	  {OV5640_TIMING_DVPHO_HIGH, 0x01},
	  {OV5640_TIMING_DVPHO_LOW, 0xE0},
	  {OV5640_TIMING_DVPVO_HIGH, 0x01},
	  {OV5640_TIMING_DVPVO_LOW, 0x10},
	};

	/* Initialization sequence for QVGA resolution (320x240) */
	static const uint16_t OV5640_QVGA[][2] =
	{
	  {OV5640_TIMING_DVPHO_HIGH, 0x01},
	  {OV5640_TIMING_DVPHO_LOW, 0x40},
	  {OV5640_TIMING_DVPVO_HIGH, 0x00},
	  {OV5640_TIMING_DVPVO_LOW, 0xF0},
	};

	/* Initialization sequence for QQVGA resolution (160x120) */
	static const uint16_t OV5640_QQVGA[][2] =
	{
	  {OV5640_TIMING_DVPHO_HIGH, 0x00},
	  {OV5640_TIMING_DVPHO_LOW, 0xA0},
	  {OV5640_TIMING_DVPVO_HIGH, 0x00},
	  {OV5640_TIMING_DVPVO_LOW, 0x78},
	};

	/* Initialize OV5640 */
	switch (resolution)
	{
	case OV5640_R160x120:
		for (index = 0; index < (sizeof(OV5640_QQVGA) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_QQVGA[index][1];
			ov5640_write_reg(OV5640_QQVGA[index][0], &tmp, 1);
		}
		break;

	case OV5640_R320x240:
		for (index = 0; index < (sizeof(OV5640_QVGA) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_QVGA[index][1];
			ov5640_write_reg(OV5640_QVGA[index][0], &tmp, 1);
		}
		break;

	case OV5640_R480x272:
		for (index = 0; index < (sizeof(OV5640_480x272) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_480x272[index][1];
			ov5640_write_reg( OV5640_480x272[index][0], &tmp, 1);
		}
		break;

	case OV5640_R640x480:
		for (index = 0; index < (sizeof(OV5640_VGA) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_VGA[index][1];
			ov5640_write_reg(OV5640_VGA[index][0], &tmp, 1);
		}
		break;
	case OV5640_R800x480:
		for (index = 0; index < (sizeof(OV5640_WVGA) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_WVGA[index][1];
			ov5640_write_reg(OV5640_WVGA[index][0], &tmp, 1);
		}
		break;
	default:
		return OV5640_ERROR;
		break;
	}
	return OV5640_OK;
}

int32_t ov5640_SetPixelFormat(uint32_t PixelFormat)
{
	uint32_t index;
	uint8_t tmp;

	/* Initialization sequence for RGB565 pixel format */
	static const uint16_t OV5640_PF_RGB565[][2] =
	{
	  {OV5640_FORMAT_CTRL00, 0x6F},
	  {OV5640_FORMAT_MUX_CTRL, 0x01},
	};

	/* Initialization sequence for YUV422 pixel format */
	static const uint16_t OV5640_PF_YUV422[][2] =
	{
	  {OV5640_FORMAT_CTRL00, 0x30},
	  {OV5640_FORMAT_MUX_CTRL, 0x00},
	};

	/* Initialization sequence for RGB888 pixel format */
	static const uint16_t OV5640_PF_RGB888[][2] =
	{
	  {OV5640_FORMAT_CTRL00, 0x23},
	  {OV5640_FORMAT_MUX_CTRL, 0x01},
	};

	/* Initialization sequence for Monochrome 8bits pixel format */
	static const uint16_t OV5640_PF_Y8[][2] =
	{
	  {OV5640_FORMAT_CTRL00, 0x10},
	  {OV5640_FORMAT_MUX_CTRL, 0x00},
	};

	/* Initialization sequence for JPEG format */
	static const uint16_t OV5640_PF_JPEG[][2] =
	{
	  {OV5640_FORMAT_CTRL00, 0x30},
	  {OV5640_FORMAT_MUX_CTRL, 0x00},
	};


	/* Set specific parameters for each PixelFormat */
	switch (PixelFormat)
	{
	case OV5640_YUV422:
		for (index = 0; index < (sizeof(OV5640_PF_YUV422) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_PF_YUV422[index][1];
			ov5640_write_reg(OV5640_PF_YUV422[index][0], &tmp, 1);
		}
		break;

	case OV5640_RGB888:
		for (index = 0; index < (sizeof(OV5640_PF_RGB888) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_PF_RGB888[index][1];
			ov5640_write_reg(OV5640_PF_RGB888[index][0], &tmp, 1);
		}
		break;

	case OV5640_Y8:
		for (index = 0; index < (sizeof(OV5640_PF_Y8) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_PF_Y8[index][1];
			ov5640_write_reg(OV5640_PF_Y8[index][0], &tmp, 1);
		}
		break;

	case OV5640_JPEG:
		for (index = 0; index < (sizeof(OV5640_PF_JPEG) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_PF_JPEG[index][1];
			ov5640_write_reg(OV5640_PF_JPEG[index][0], &tmp, 1);
		}
		break;

	case OV5640_RGB565:
		for (index = 0; index < (sizeof(OV5640_PF_RGB565) / 4U); index++)
		{
			tmp = (uint8_t) OV5640_PF_RGB565[index][1];
			ov5640_write_reg(OV5640_PF_RGB565[index][0], &tmp, 1);
		}
		break;

	default:
		return OV5640_ERROR;
	}
	return OV5640_OK;
}

int32_t ov5640_SetPolarities(uint32_t PclkPol, uint32_t HrefPol, uint32_t VsyncPol)
{
	uint8_t tmp;
	tmp = (uint8_t)(PclkPol << 5U) | (HrefPol << 1U) | VsyncPol;
	ov5640_write_reg(OV5640_POLARITY_CTRL, &tmp, 1);

	return OV5640_OK;
}

int32_t ov5640_enableTestMode()
{
	uint8_t tmp = 0b10000000;
	ov5640_write_reg(OV5640_PRE_ISP_TEST_SETTING1, &tmp, 1);
	return OV5640_OK;
}
