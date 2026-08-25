#ifndef __UDS_DATA_H__
#define __UDS_DATA_H__

#include "PlatformTypes.h"


typedef struct{
    uint16 DID;
    
    /*Security Access shall be added*/
    uint8 datalen;
    uint8* data;
} unimax_UdsDataType;


#endif /* __UDS_DATA_H__ */