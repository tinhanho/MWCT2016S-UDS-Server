/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : S32K3XX
*   Dependencies         : none
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.0
*   Build Version        : S32K3_RTD_2_0_0_D2111_CD01_ASR_REL_4_4_REV_0000_20211129
*
*   (c) Copyright 2020 - 2021 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file main.c
*
*   @addtogroup main_module main module documentation
*   @{
*/

/* Including necessary configuration files. */
//#include "includes.h"
#include "standardTypes.h"
#include "Clock_Ip.h"
#include "Clock_Ip_Cfg.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Port_Ip_Cfg.h"
#include "FlexCAN_Ip.h"
#include "IntCtrl_Ip.h"
#include "C40_Ip.h"
//#include "Mem_43_INFLS.h"
//#include "MemAcc.h"
/* User includes */

#include "bootloader_main.h"
#include "TP.h"



volatile int exit_code = 0;
#define CAN_MSG_TYPE (CAN_MSG_ID_STD)
#define RX_MAILBOX_ID (1u)
#define TX_MAILBOX_ID (2u)
Flexcan_Ip_MsgBuffType g_RXCANMsg;

extern void CAN0_ORED_0_31_MB_IRQHandler(void);
extern void Swt_Ip_Swt0_Isr(void);

const Flexcan_Ip_DataInfoType RXCANMsgConfig = 
{
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 64u,
    .fd_enable   = TRUE,
    .enable_brs  = TRUE,
    .is_polling = FALSE,
    .is_remote = FALSE
};

Flexcan_Ip_DataInfoType TXCANMsgConfig = 
{
    .msg_id_type = FLEXCAN_MSG_ID_STD,
    .data_length = 8u,
    .is_polling = FALSE,
    .is_remote = FALSE
};

/**
 * @brief 
 * 
 * @param instance 
 * @param eventType 
 * @param buffIdx 
 * @param flexcanState 
 */
void flexcan0_Callback(uint8 instance,Flexcan_Ip_EventType eventType,
                      uint32 buffIdx,const Flexcan_Ip_StateType * flexcanState)
{
    
    if(FLEXCAN_EVENT_RX_COMPLETE == eventType)
    {   
        TP_DriverWriteDataInTP(g_RXCANMsg.msgId, g_RXCANMsg.dataLen, g_RXCANMsg.data);
        FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MAILBOX_ID, &g_RXCANMsg, FALSE);
    }
    else if(FLEXCAN_EVENT_TX_COMPLETE == eventType)
    {
        TP_DoTxMsgSuccesfulCallback();
    }
    else
    {}
}
/**
 * @brief 
 * 
 */
void SendMsg(void)
{
	Flexcan_Ip_MsgBuffType txMsg;
	Flexcan_Ip_StatusType status = FLEXCAN_STATUS_SUCCESS;
    uint32 dataLen;
    status = FlexCAN_Ip_GetTransferStatus(INST_FLEXCAN_0, TX_MAILBOX_ID);
	if(FLEXCAN_STATUS_SUCCESS == status)
	{
		if(TRUE == TP_DriverReadDataFromTP(sizeof(txMsg.data), txMsg.data, &(txMsg.msgId), &(dataLen)))
		{
            TXCANMsgConfig.data_length = dataLen;
            FlexCAN_Ip_Send(INST_FLEXCAN_0, TX_MAILBOX_ID, &TXCANMsgConfig, txMsg.msgId, txMsg.data);
		}
	}
}

/**
 * @brief 
 * 
 */
void AbortTransferMsg(void)
{
    FlexCAN_Ip_AbortTransfer(INST_FLEXCAN_0,TX_MAILBOX_ID);
}

/**
 * @brief 
 * 
 */
static void BSP_init(void)
{
    Clock_Ip_Init(&Clock_Ip_aClockConfig[0]);
    Siul2_Port_Ip_Init(NUM_OF_CONFIGURED_PINS_PortContainer_0_VS_0,g_pin_mux_InitConfigArr_PortContainer_0_VS_0);
    C40_Ip_Init(&C40_Ip_InitCfg);
    //Mem_43_INFLS_Init(&Mem_43_INFLS_Config);
    //MemAcc_Init(&MemAcc_Config);
//    IntCtrl_Ip_Init(&IntCtrlConfig_0);
//    IntCtrl_Ip_ConfigIrqRouting(&intRouteConfig);
    IntCtrl_Ip_EnableIrq(FlexCAN0_1_IRQn);
	IntCtrl_Ip_InstallHandler(FlexCAN0_1_IRQn, CAN0_ORED_0_31_MB_IRQHandler, NULL_PTR);
//    IntCtrl_Ip_EnableIrq(LPUART6_IRQn);
//    IntCtrl_Ip_InstallHandler(LPUART6_IRQn, LPUART_UART_IP_6_IRQHandler, NULL_PTR);
    IntCtrl_Ip_InstallHandler(SWT0_IRQn, Swt_Ip_Swt0_Isr, NULL_PTR);
	FlexCAN_Ip_Init(INST_FLEXCAN_0, &FlexCAN_State0, &FlexCAN_Config0);
	FlexCAN_Ip_SetStartMode(INST_FLEXCAN_0);
	FlexCAN_Ip_ConfigRxMb(INST_FLEXCAN_0, RX_MAILBOX_ID, &RXCANMsgConfig, RX_PHY_ID);
	FlexCAN_Ip_Receive(INST_FLEXCAN_0, RX_MAILBOX_ID, &g_RXCANMsg, FALSE);

    //Disable FRET
    //((Power_Ip_MC_RGM_Type *)IP_MC_RGM_BASE)->FRET = 0;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your code here */
    BOOTLOADER_MAIN_Init(&BSP_init, &AbortTransferMsg);
    for(;;)
    {
        BOOTLOADER_MAIN_Demo();
        SendMsg();
        if(exit_code != 0)
        {
            break;
        }
    }
    return exit_code;
}

/** @} */
