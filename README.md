# MWCT2016S-UDS-Server

> Reference: check [nxp](https://community.nxp.com/t5/S32K-Knowledge-Base/Unified-bootloader-Demo/ta-p/1423099)

## Requirements

- S32K312 SDK and configuration tool with C40_Ip and Power_Ip support.
- ECUBus for UDS test execution.
- PCAN tools and a CAN interface. See [PCAN Tutorial.pdf](PCAN%20Tutorial.pdf).

## Project Layout

- `application/`: Application-side FIFO, transport protocol, and UDS stack.
- `can_bootloader/`: CAN bootloader, Flash services, HAL, and UDS stack.
- `bin/S32K312_CAN_bootloader_RTD2d0.bin`: Built CAN bootloader image.
- `ECUBus Config/Boot_Test/config.json`: ECUBus Boot_Test configuration.

Install ECUBus and configure the CAN hardware by following the PCAN tutorial before running the examples.

## Application

1. Import the modules under `application/FIFO` and `application/UDS_stack` into the application project.
2. Add ```TP_SystemTickCtl``` and ```UDS_SystemTickCtl``` in 1ms task.
```c
static void SampleAppTask1ms(uint16 wNumTicks)
{
    ...
	TP_SystemTickCtl();
	UDS_SystemTickCtl();
    ...
}
```
3. Register RX_MAIL, TX_MAIL, RX message configuration, and TX message configuration.
4. Call ```TP_Init``` and ```UDS_Init``` in `main.c`.
5. Add the TP, UDS, and SendMsg main functions to the main loop and set the FlexCAN callback function.
    - Implementations of SendMsgMainFun and flexcan0_Callback can be located in `main.c`.

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

## CAN Bootloader

> Use the Boot_Test project instead of S32K312_user_config.

1. Complete the steps in [Application](#application).
2. Import the modules under `can_bootloader/` into the bootloader project.
3. Add the C40_Ip and Power_Ip modules in the configuration tool.
4. Configure the CAN transport layer and application memory range for the target.
5. Use `bin/S32K312_CAN_bootloader_RTD2d0.bin` as the bootloader image when programming the target.
6. Open `ECUBus Config/Boot_Test/config.json` in ECUBus to run the bootloader test sequence.

## TODO

- Upload the memory map configuration.