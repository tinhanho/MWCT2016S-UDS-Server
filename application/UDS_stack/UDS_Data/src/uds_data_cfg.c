#include "uds_data.h"
#include "uds_data_cfg.h"

uint8 ECUSOFTWARENUMBER[] = "UNIMAX_001";

const static unimax_UdsDataType UdsDataSet[] = {
    {
        .DID = VEHICLEMANUFACTURERECUSOFTWARENUMBERDATAIDENTIFIER,
        .datalen = 10,
        .data = ECUSOFTWARENUMBER
    }
};

unimax_UdsDataType* unimax_UDS_GetDataInfo(uint8* idsNum){
    *idsNum = sizeof(UdsDataSet) / sizeof(UdsDataSet[0u]);

    return (unimax_UdsDataType*) &UdsDataSet[0u];
}
