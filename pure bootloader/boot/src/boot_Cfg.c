/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 * @file boot_Cfg.c
 * 
 * @brief: Add your description here for this file.
 *
 * @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Tomlin Tang  2019-01-31 09:51:53  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */


/*******************************************************************************
 * User Include
 ******************************************************************************/
#include "boot_cfg.h"
#include "flash_hal_cfg.h"
#include "fls_app.h"
#include "CRC_hal.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
// typedef struct
// {
// 	uint8 infoDataLen;             /*Exchange inforamtion length must N * 4.*/
// 	uint8 requestEnterBootloader;  /*Request enter bootloader mode flag*/
// 	uint8 downloadAPPSuccessful;   /*downlaod APP successful flag*/
// 	uint32 infoStartAddr;          /*exchange information start address*/
// 	uint32 requestEnterBootloaderAddr; /*Request enter bootloader flag address */
// 	uint32 downloadAppSuccessfulAddr;  /*download APP successful flag address*/
// }tBootInfo;

// const static tBootInfo gs_stBootInfo = {
// 	16u,    /*Exchange inforamtion length must N * 4.*/
// 	0x5Au,  
// 	0xA5u,
// 	0x20417FF0u,
// 	0x20417FF1u,
// 	0x20417FF0u
// };

// /*get information storage CRC*/
// #define GetInfoStorageCRC() (*(uint16 *)(gs_stBootInfo.infoStartAddr + 14))

// /*set information CRC */
// #define SetInforCRC(xCrc) ((*(uint16 *)(gs_stBootInfo.infoStartAddr + 14)) = (uint16)(xCrc))

// /*Is information valid?*/
// static boolean Boot_IsInfoValid(void);

// /*calculate information CRC*/
// static uint16 Boot_CalculateInfoCRC(void);

// /*set download app successful */
// void Boot_SetDownloadAppSuccessful(void)
// {
// 	uint16 infoCrc = 0u;

// 	*((uint8 *)gs_stBootInfo.downloadAppSuccessfulAddr) = gs_stBootInfo.downloadAPPSuccessful;

// 	infoCrc = Boot_CalculateInfoCRC();
// 	SetInforCRC(infoCrc);
// }

// /*Is request enter bootloader?*/
// boolean Boot_IsRequestEnterBootloader(void)
// {
// 	boolean result = FALSE;

// 	if(TRUE == Boot_IsInfoValid())
// 	{
// 		if(gs_stBootInfo.requestEnterBootloader == *((uint8 *)gs_stBootInfo.requestEnterBootloaderAddr))
// 		{
// 			result = TRUE;
// 		}
// 	}
// 	return result;
// }

// /*clear request enter bootloader flag*/
// void Boot_ClearRequestEnterBootloaderFlag(void)
// {
// 	uint16 infoCrc = 0u;

// 	*((uint8 *)gs_stBootInfo.requestEnterBootloaderAddr) = 0u;

// 	infoCrc = Boot_CalculateInfoCRC();
// 	SetInforCRC(infoCrc);
// }

/*Is power on trigger reset?*/
//boolean Boot_IsPowerOnTriggerReset(void)
//{
//	boolean result = FALSE;
//	if( (MCU_SWT0_RST_RESET != Power_Ip_GetResetReason()) ||
//		(MCU_WAKEUP_REASON  == Power_Ip_GetResetReason()))
//	{
//		result = TRUE;
//	}
//
//	return result;
//}

/*when power on, clear all flag in RAM for ECC.*/
//void Boot_PowerONClearAllFlag(void)
//{
//	uint16 infoCrc = 0u;
//	uint8 index = 0u;
//
//	/*clear RAM with 4 bytes for ECC*/
//	for(index = 0u; index < (gs_stBootInfo.infoDataLen >> 2u); index++)
//	{
//		*((uint32 *)gs_stBootInfo.infoStartAddr + index) = 0u;
//	}
//
//	infoCrc = Boot_CalculateInfoCRC();
//	SetInforCRC(infoCrc);
//}

/*remap multi-core application*/
void Boot_RemapApplication(void)
{
	uint32 totalCoreNo = 0u;
	uint32 index = 0u;
	tAPPType appType = APP_A_TYPE;
	uint32 appMirrorAddr = 0u;
	uint32 appRemapAddr = 0u;
	
	totalCoreNo = FLASH_HAL_GetConfigCoreNo();
	if(totalCoreNo > 0u)
	{
		appType = Flash_GetNewestAPPType();
		
		for(index = 0u; index < totalCoreNo; index++)
		{
			if((TRUE == FLASH_HAL_GetMultiCoreMirrorAddr(appType,index,&appMirrorAddr)) &&
				(TRUE == FLASH_HAL_GetMultiCoreRemapAddr(appType,index,&appRemapAddr)))
			{
				/*do remap multi core application*/
			}
			else
			{
				/*trigger MCU reset*/
			}
		}
	}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Boot_JumpToApp
 * Description   : This function is jump to app.
 *
 * Implements : 
 *END**************************************************************************/
typedef void (*AppAddr)(void);
AppAddr JumpAppAddr = NULL;
void Boot_JumpToApp(const uint32 i_AppAddr)
{
	AppAddr resetHandle = (AppAddr)((i_AppAddr /*+ 0x420*/) | 0x01);

	(resetHandle)();

#if 0
	/*example for CodeWarroir -- MagniV*/
	AppAddr appAddr;
		
	__asm{
		LD D6, i_AppAddr
		TFR D6, X
		ST X, appAddr
		JSR [appAddr]
	}
#endif
}

// /*Is information valid?*/
// static boolean Boot_IsInfoValid(void)
// {
// 	uint16 infoCrc = 0u;
// 	uint16 storageCrc = 0u;
// 	boolean result = FALSE;
	
// 	infoCrc = Boot_CalculateInfoCRC();

// 	storageCrc = GetInfoStorageCRC();

// 	if(storageCrc == infoCrc)
// 	{
// 		result = TRUE;
// 	}

// 	return result;
// }

// /*calculate information CRC*/
// static uint16 Boot_CalculateInfoCRC(void)
// {
// 	uint32 infoCrc = 0u;

// 	CRC_HAL_CalculateCRCOnce((const uint8 *)gs_stBootInfo.infoStartAddr, gs_stBootInfo.infoDataLen - 2u, &infoCrc);

// 	return (uint16)infoCrc;
// }


/******************************************************************************
 * EOF
 *****************************************************************************/
