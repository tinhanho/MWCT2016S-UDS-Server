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
 * @file bootloader_debug.c
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
 V1.0.0  Tomlin Tang  2019-01-17 15:52:56  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */
#include "bootloader_debug.h"
#include "user_config.h"
#include "Lpuart_Uart_Ip.h"

#ifdef EN_DEBUG_PRINT
/*here include platform headers for driver*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/*****************************************/
#endif

/*******************************************************************************
 * User Include8
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : BOOTLOADER_DEBUG_Init
 * Description   : This function initial this module.
 *
 * Implements : BOOTLOADER_DEBUG_Init_Activity
 *END**************************************************************************/
void BOOTLOADER_DEBUG_Init(void)
{
	Debug_InitDebugIO();
	Debug_InitDebugTimer();
	Debug_InitDebugPrint();

}

#ifdef EN_DEBUG_PRINT
void Bootloader_DebugPrintInit(void)
{
	Lpuart_Uart_Ip_Init(1, &Lpuart_Uart_Ip_xHwConfigPB_1);
	Lpuart_Uart_Ip_Init(0, &Lpuart_Uart_Ip_xHwConfigPB_0);
	Lpuart_Uart_Ip_Init(3, &Lpuart_Uart_Ip_xHwConfigPB_3);
}
void Bootloader_DebugPrint(const char *fmt, ...)
{

	static char buffer[DEBUG_LOG_BUF_SIZE];
	
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buffer, DEBUG_LOG_BUF_SIZE, fmt, ap);
	va_end(ap);
	Lpuart_Uart_Ip_SyncSend(1, (const uint8 *)buffer, strlen(buffer),100000u);
}
#endif

/******************************************************************************
 * EOF
 *****************************************************************************/
