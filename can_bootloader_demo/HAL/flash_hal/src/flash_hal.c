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
 * @file flash_hal.c
 * 
 * @brief: Add your description here for this file.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section Rule_X-Y Rule: X.Y (Advisory/Required)
 * Violates MISRA 2012 Advisory Rule X.Y, Rule description here.
 *
 * @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Tomlin Tang  2019-01-17 15:51:03  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */

#include "flash_hal.h"
#include "C40_Ip.h"

/*******************************************************************************
 * User Include
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static boolean FLASH_HAL_Init(void);

static boolean FLASH_HAL_EraseSector(const uint32 i_startAddr, const uint32 i_noEraseSectors);

static boolean FLASH_HAL_WriteData(const uint32 i_startAddr,
								 const uint8 *i_pDataBuf, 
								 const uint32 i_dataLen);

static boolean FLASH_HAL_ReadData(const uint32 i_startAddr,
								const uint32 i_readLen, 
					    		uint8 *o_pDataBuf);

static void FLASH_HAL_Deinit(void);

extern void C40_Ip_StartSequenceInit(uint8_t* FlsDrv);
/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_Init
 * Description   : This function initial this module.
 *
 * Implements : FLASH_HAL_Init_Activity
 *END**************************************************************************/
static boolean FLASH_HAL_Init(void)
{
	uint32 FlashDriverAddrStart,FlashDriverEndAddr;
	FLASH_HAL_GetFlashDriverInfo(&FlashDriverAddrStart,&FlashDriverEndAddr);
	//Fls_Init((uint8*)(FlashDriverAddrStart),FlashDriverEndAddr-FlashDriverAddrStart);
	//C40_Ip_StartSequenceInit((uint8*)(FlashDriverAddrStart));

	//C40_Ip_Init(&C40ConfigSet_BOARD_InitPeripherals_InitCfg);
	C40_Ip_Init(&C40_Ip_InitCfg);  /*  change for mcal  */ 
    return TRUE;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_EraseSector
 * Description   : This function is erase flash sectors.
 * Parameters	  :  i_startAddr input for start flash address
                        i_noEraseSectors input number of erase sectors
 * Implements : FLASH_HAL_Init_Activity
 *END**************************************************************************/
static boolean FLASH_HAL_EraseSector(const uint32 i_startAddr, const uint32 i_noEraseSectors)
{
    boolean retstates = FALSE;        /* Store the driver APIs return code */
	uint8 ret = 0u;
	uint32 sectorLength = 0u;
	uint32 i = 0u;
	C40_Ip_StatusType c40Status,c40Res;
	C40_Ip_VirtualSectorsType VirtualSector;
	sectorLength = FLASH_HAL_Get1SectorBytes();
	//The program flash start address is 0x400000u, 32 is the program flash VirtualSector begin
	VirtualSector = (i_startAddr - 0x400000u)/sectorLength + C40_IP_MAX_DATA_SECTOR;
	for(i=0;i<i_noEraseSectors;i++)
	{
		if(C40_IP_STATUS_SECTOR_PROTECTED == C40_Ip_GetLock(VirtualSector+i))
		{
			C40_Ip_ClearLock(VirtualSector+i, 0);
		}
		/* Erase sector */
		DisableAllInterrupts();
		c40Res = C40_Ip_MainInterfaceSectorErase(VirtualSector+i, 0);
		do
		{
			c40Status = C40_Ip_MainInterfaceSectorEraseStatus();
		}
		while (C40_IP_STATUS_BUSY == c40Status);
		ASM_KEYWORD(" dsb ");
		ASM_KEYWORD(" isb ");

		/* 延遲約 10~20 微秒，讓 Flash 硬體控制器完成 Pipeline 與 Bus 復位 */
		for (volatile uint32 delay = 0; delay < 1000; delay++)
		{
			ASM_KEYWORD(" nop ");
		}
		EnableAllInterrupts();
		if((C40_IP_STATUS_SUCCESS == c40Res) && (C40_IP_STATUS_SUCCESS == c40Status))
		{
			retstates = TRUE;
		}
		else
		{
			retstates = FALSE;
			break;
		}
	}
	return retstates;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_WriteData
 * Description   : This function is write data in flash. if write write data successfull return TRUE, else return FALSE.
 * Parameters	  :  i_startAddr input for start flash address
                        i_pDataBuf write data buf
                        i_dataLen write data len
 * Implements : FLASH_HAL_Init_Activity
 *END**************************************************************************/

static boolean FLASH_HAL_WriteData(const uint32 i_startAddr,
								   const uint8 *i_pDataBuf, 
								   const uint32 i_dataLen)
{
    boolean retstates = FALSE;
	uint8 lessWriteLen = 8u;
	uint8 aDataBuf[8u] = {0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu};
	uint32 writeDataLen = 0u;
	uint8 index = 0u;

	uint32 sectorLength = 0u;
	C40_Ip_StatusType c40Status,c40Res;
	C40_Ip_VirtualSectorsType VirtualSector;
	C40_Ip_VirtualSectorsType endVirtualSector;
	
	sectorLength = FLASH_HAL_Get1SectorBytes();
	//The program flash start address is 0x400000u, 32 is the program flash VirtualSector begin
	VirtualSector = (i_startAddr - 0x400000u)/sectorLength + C40_IP_MAX_DATA_SECTOR;
	endVirtualSector = ((i_startAddr + i_dataLen) - 0x400000u) / sectorLength + C40_IP_MAX_DATA_SECTOR;

	while(VirtualSector <= endVirtualSector)
	{
		if(C40_IP_STATUS_SECTOR_PROTECTED == C40_Ip_GetLock(VirtualSector))
		{
			C40_Ip_ClearLock(VirtualSector, 0);
		}

		VirtualSector++;
	}

	DisableAllInterrupts();
	if(i_dataLen  & (lessWriteLen - 1))
	{
		/*if write data more than 8 bytes*/
		if(i_dataLen > lessWriteLen)
		{
			writeDataLen = i_dataLen - (i_dataLen  & (lessWriteLen - 1));
			if(C40_IP_STATUS_SUCCESS == C40_Ip_MainInterfaceWrite(i_startAddr,writeDataLen,i_pDataBuf, 0))
		    {
		    	retstates = TRUE;
		    }
			else
			{
				retstates = FALSE;
			}

			if((TRUE == retstates))
			{
				for(index = 0u; index < (i_dataLen  & (lessWriteLen - 1)); index++)
				{
					aDataBuf[index] = i_pDataBuf[writeDataLen + index];
				}
				if(C40_IP_STATUS_SUCCESS == C40_Ip_MainInterfaceWrite(i_startAddr+writeDataLen,8u,aDataBuf,0))
			    {
			    	retstates = TRUE;
			    }
			}
		}
		else
		{
			for(index = 0u; index < i_dataLen; index++)
			{
				aDataBuf[index] = i_pDataBuf[writeDataLen + index];
			}
			if(C40_IP_STATUS_SUCCESS == C40_Ip_MainInterfaceWrite(i_startAddr+writeDataLen,8,aDataBuf,0)) /*  这个地方应该为aDataBuf */
		    {
		    	retstates = TRUE;
		    }			
		}
	}
	else
	{
		writeDataLen = 0u;
		retstates = TRUE;
		while(writeDataLen < i_dataLen)
		{
			if(C40_IP_STATUS_SUCCESS != C40_Ip_MainInterfaceWrite(i_startAddr + writeDataLen, lessWriteLen, i_pDataBuf + writeDataLen, 0))
		    {
				retstates = FALSE;
				
				break;
		    }

			// unimax debug
			while(C40_IP_STATUS_SUCCESS != C40_Ip_MainInterfaceWriteStatus())
			{
				// do nothing 
			}

			writeDataLen += lessWriteLen;
		}
	}
	EnableAllInterrupts();
	
	return retstates;

}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_ReadData
 * Description   : This function is read data in RAM. if read data successfull return TRUE, else return FALSE.
 * Parameters	  :  i_startAddr input for start flash address
                        i_readLen read data length
                        o_pDataBuf read data buf
 * Implements : FLASH_HAL_Init_Activity
 *END**************************************************************************/
static boolean FLASH_HAL_ReadData(const uint32 i_startAddr,
								const uint32 i_readLen, 
					    		uint8 *o_pDataBuf)
{
	uint32 i;
	//FLS_DebugPrintf("\n %s\n", __func__);
    //ReadFlashMemory(i_startAddr, i_readLen, o_pDataBuf);
	for(i=0;i<i_readLen;i++)
	{
		o_pDataBuf[i] = ((uint8*)(i_startAddr))[i];
	}
	return TRUE;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_Deinit
 * Description   : This function initial this module.
 *
 * Implements : FLASH_HAL_Deinit_Activity
 *END**************************************************************************/
static void FLASH_HAL_Deinit(void)
{
	//FLS_DebugPrintf("\n %s\n", __func__);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_HAL_RegisterFlashAPI
 * Description   : This function is register flash API. The API maybe download from host and storage in RAM.
 * 
 *END**************************************************************************/
boolean FLASH_HAL_RegisterFlashAPI(tFlashOperateAPI *o_pstFlashOperateAPI)
{
	boolean result = FALSE;

	if(NULL_PTR != o_pstFlashOperateAPI)
	{
		o_pstFlashOperateAPI->pfFlashInit = FLASH_HAL_Init;
		o_pstFlashOperateAPI->pfEraserSecotr = FLASH_HAL_EraseSector;
		o_pstFlashOperateAPI->pfProgramData = FLASH_HAL_WriteData;
		o_pstFlashOperateAPI->pfReadFlashData = FLASH_HAL_ReadData;
		o_pstFlashOperateAPI->pfFlashDeinit = FLASH_HAL_Deinit;

		result = TRUE;
	}

	return result;
}

/******************************************************************************
 * EOF
 *****************************************************************************/
