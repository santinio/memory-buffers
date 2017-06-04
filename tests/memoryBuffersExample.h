/*
 * memoryBuffersExample.h
 *
 *  Created on: 3 Jun 2017
 *      Author: lykos
 */

#ifndef MEMORYBUFFERSEXAMPLE_H_
#define MEMORYBUFFERSEXAMPLE_H_

#include <stdint.h>

typedef enum{
    DATA_BUFFER_ERROR=-1,
    DATA_BUFFER_OK,
    DATA_BUFFER_OVERFLOW,
    DATA_BUFFER_EMPTY
}PACKET_STATUS;

typedef struct{
    char *msg;
    uint8_t size;
}messagePacket_t;

/*This function will take a string, package it as a message_packet_t struct and attempt
 * to add it in the linked list buffer. It will notify the caller about the result.
 */
PACKET_STATUS messageAdd(char *);
/*This function will read the first packet from the linked list and notify the caller
 * about the result.
 */
PACKET_STATUS messageGet();
/*This function will allocate the memory as required by the messagePacket_t struct.
 * NOT USED in this example.
 */
void messageAlloc(void **);
/*This function will free the memory as required by the messagePacket_t struct.
 */
void messageFree(void **);

#endif /* MEMORYBUFFERSEXAMPLE_H_ */
