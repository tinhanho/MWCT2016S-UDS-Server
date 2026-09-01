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

#ifndef FLASH_HAL_H
#define FLASH_HAL_H


#include "flash_hal_Cfg.h"
/*******************************************************************************
 * User Include
 ******************************************************************************/

/*! 
 * @file: flash_hal.h
 *
 * @brief: Add your description here for this file.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section Rule_X-Y Rule: X.Y (Required)
 * Violates MISRA 2012 Required Rule X.Y, Rule description here.
 *
 * @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Tomlin Tang  2019-01-17 15:51:03  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */

/*!
 * @addtogroup flash_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#if defined (__cplusplus)
extern "C" {
#endif

typedef boolean (*tpfFlashInit)(void);
typedef void (*tpfFlashDeInit)(void);
typedef boolean (*tpfEraseSecotr)(const uint32, const uint32);
typedef boolean (*tpfProgramData)(const uint32, const uint8 *, const uint32);
typedef boolean (*tpfReadFlashData)(const uint32, const uint32, uint8 *);

typedef struct
{
	tpfFlashInit pfFlashInit;
	tpfEraseSecotr pfEraserSecotr;     /*erase sector*/
	tpfProgramData pfProgramData;      /*program data*/
	tpfReadFlashData pfReadFlashData;  /*read flash data*/	
	tpfFlashDeInit pfFlashDeinit;
}tFlashOperateAPI;


/*******************************************************************************
 * API
 ******************************************************************************/
 
/*! @}*/
/*!
 * @name Register Flash API
 * Register opeater FLASH API.
 *
 * To disable this module, you need call another function (see \ref Uninitialize "Disable Module")
 */
/*! @{*/

/*!
 * @brief To Register operate API.
 *
 * This function returns the state of the initial.
 *
 * @param[out] operate flash API 
 * @return register status.
 */ 
extern boolean FLASH_HAL_RegisterFlashAPI(tFlashOperateAPI *o_pstFlashOperateAPI);

/*! @}*/

#if defined (__cplusplus)
}
#endif

/*! @}*/

#endif /* FLASH_HAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
