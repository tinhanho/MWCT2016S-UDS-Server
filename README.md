# MWCT2016S-UDS-Server


## Installaltion

- Follow the PCAN tutorial and install ECUBus

## App Demo

1. Import FIFO and UDS_Stack
2. Add ```TP_SystemTickCtl``` and ```UDS_SystemTickCtl``` in 1ms task
```c
static void SampleAppTask1ms(uint16 wNumTicks)
{
    ...
	TP_SystemTickCtl();
	UDS_SystemTickCtl();
    ...
}
```
3. Register RX_MAIL, TX_MAIL, RX message configuration, TX message configuration
4. Call ```TP_Init``` and ```UDS_Init``` in main.c
5. Add TP, UDS and SendMsg mainfunction in main loop and set flexcan callback function
    - Implementations of SendMsgMainFun and flexcan0_Callback can be located in main.c

```c
/*Send msg main function*/
static void SendMsgMainFun(void)
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
```
```c
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
```

## CAN Bootloader Demo

> Use the Boot_Test project rather instead of S32K312_user_config

1. Do all the steps in [App Demo](#app-demo)
2. Import boot, Flash_app, bootmain.c .h and HAL in src and update UDS_stack
3. Add C40_Ip, Power_Ip module in configuration tool

<!-- 
Flash driver in RAM needs further step and proper binary code for flash driver
4. Update C40_Ip.c
```c
typedef C40_Ip_StatusType (*C40_Ip_StartSequenceType)(uint8_t Operation,uint32_t TimeoutCnt);
C40_Ip_StartSequenceType C40_Ip_StartSequencePtr = NULL;

void C40_Ip_StartSequenceInit(uint8_t* FlsDrv)
{
    //blx cmd address need to add 1
    C40_Ip_StartSequencePtr = (C40_Ip_StartSequenceType)(((uint32_t)FlsDrv)+1);
} 
```
!-->