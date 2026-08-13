# MWCT2016S-UDS-Server

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
5. Add TP, UDS and SendMsg mainfunction in main loop

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