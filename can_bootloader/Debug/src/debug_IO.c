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
 * @file debug_IO.c
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
 V1.0.0  Tomlin Tang  2019-01-23 10:33:26  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
 */

#include "user_config.h"

/*******************************************************************************
 * User Include
 ******************************************************************************/
#ifdef EN_DEBUG_IO
/*here include platform headers for driver*/

/*****************************************/
#endif
#include "debug_IO.h"
#include "Siul2_Dio_Ip.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef EN_DEBUG_IO
/*
 *  LED IO --->
 *  	PTA29 -- BLUE
 *  	PTA30 -- GREEN
 *  	PTA31 -- RED
 *  	L_HALF: [0, 15]
 *  	H_HALF: [16,31]
 *
 *  	PTA29
 */
#define LED_PORT (PTA_H_HALF)
#define LED_PIN_NO_START (16)

#define BLUE_PIN_NO (29 - LED_PIN_NO_START)
#define GREEN_PIN_NO (30 - LED_PIN_NO_START)
#define RED_PIN_NO (31 - LED_PIN_NO_START)

#define LED_PIN ((1u << (GREEN_PIN_NO)))

/*FUNCTION**********************************************************************
 *
 * Function Name : DEBUG_IO_Init
 * Description   : This function initial this module.
 *
 * Implements : DEBUG_IO_Init_Activity
 *END**************************************************************************/
void DEBUG_IO_Init(void)
{

}

/*FUNCTION**********************************************************************
 *
 * Function Name : DEBUG_IO_Deinit
 * Description   : This function initial this module.
 *
 * Implements : DEBUG_IO_Deinit_Activity
 *END**************************************************************************/
void DEBUG_IO_Deinit(void)
{

}

void DEBUG_IO_SetDebugIOLow(void)
{
	Siul2_Dio_Ip_ClearPins(LED_PORT,LED_PIN);
}

void DEBUG_IO_SetDebugIOHigh(void)
{
	Siul2_Dio_Ip_SetPins(LED_PORT,LED_PIN);
}

void DEBUG_IO_ToggleDebugIO(void)
{
	Siul2_Dio_Ip_TogglePins(LED_PORT,LED_PIN);
}

#endif
/******************************************************************************
 * EOF
 *****************************************************************************/
