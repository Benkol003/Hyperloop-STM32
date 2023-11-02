#include <stdlib.h>

#include "stm32f4xx_hal_crc.h"

extern CRC_HandleTypeDef hcrc; //from main.c


/*
for STM32F401RE board the polynomial is fixed to crc32 0x04C11DB7, and initial value 0xFFFFFFFF.
XOR folding used on the upper and lower halves of the CRC32
data must be in chunks of 32 bits; if youre sending c strings/char arrays (8 bits) then you can just pack the end with zeroes
*/

#define BAUD_RATE 9600

typedef struct {
    uint16_t ID; //only use lower 11 bits
    uint8_t RTR; //=1 for data request - must also have no data
    uint32_t dataSize; //<=8
    uint32_t* data;
    short checksum; //15 bits; not a correct CRC due to the board
    short ACK;

} CAN_frame;

typedef enum{
	CAN_OK,
	CAN_ID_VALUE_RANGE_EXCEEDED,
	CAN_DATA_SIZE_EXCEEDED,

	CAN_RECIEVE_TIMEOUT,
	CAN_RECIEVE_ERRORS_EXCEEDED

} CAN_status;

uint16_t CAN_checksum(uint32_t* data, uint32_t ds){ //TODO test when we have no data
    uint32_t crc32 = HAL_CRC_Calculate(&hcrc,data,ds);
    uint16_t checksum = ( (crc32>>16)^(crc32 & 0x00FF) )>>1; //XOR each 16 bit half, and discard topmost bit for 15 bit checksum
    return checksum;
}

CAN_status CAN_send_data_frame(uint16_t ID, uint32_t data[],uint32_t dataSize){
	if(ID>0x7FF) return CAN_ID_VALUE_RANGE_EXCEEDED;
	if(dataSize>2) return CAN_DATA_SIZE_EXCEEDED;

    return CAN_OK;
}

CAN_status CAN_send_remote_frame(uint16_t ID){
	if(ID>0x7FF) return CAN_ID_VALUE_RANGE_EXCEEDED;

	return CAN_OK;
}

CAN_status CAN_listen_frame(uint16_t ID, uint32_t data[], uint32_t* dataSize){

	return CAN_OK;
}




