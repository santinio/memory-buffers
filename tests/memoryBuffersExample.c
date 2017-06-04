/*
 ============================================================================
 Name        : memoryBuffersExample.c
 Author      : vs
 Version     :
 Copyright   : No really needed
 Description : Some code to illustrate the benefits of using linked lists.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "strings.h"
#include "../src/bufferMemory.h"
#include "memoryBuffersExample.h"
#include <stdbool.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 10
#define LIST_SIZE 5
#define LOOP_RUNS 10

#define TEST_OVERFLOW_ADD 7
#define TEST_OVERFLOW_GET 2

static buffer_t buffer = {.bufferType = BUFFER_TYPE_INACTIVE};


PACKET_STATUS messageAdd(char *message){
    /*Create new dataBuffer_t object*/
    messagePacket_t *messagePacket;
    uint16_t size = 0;
    BUFFER_STATUS bStatus = 0;
    /*Allocate space for the dataBuffer*/
    messagePacket = (messagePacket_t*)calloc(sizeof(messagePacket_t),1);
    if(messagePacket==NULL)
        return DATA_BUFFER_ERROR;
    size = strlen(message);
    if(size>MAX_MESSAGE_SIZE)//If strlen overflows???
    {
        free(messagePacket);
        return DATA_BUFFER_OVERFLOW;
    }
    messagePacket->msg = (char*)calloc(size+1,1); //Adding 1 for the NULL ending
    if(messagePacket->msg==NULL)
    {
        free(messagePacket);
        return DATA_BUFFER_ERROR;
    }
    memcpy(messagePacket->msg,message,size);
    messagePacket->size = size;

    bStatus = bufferAddData(&buffer,messagePacket,1);
    if((bStatus==BUFFER_STATUS_INACTIVE)||(bStatus==BUFFER_STATUS_FULL)||(bStatus==BUFFER_STATUS_ERROR))
    {
        buffer.myFree((void**)&messagePacket);
        return DATA_BUFFER_ERROR;
    }
    return DATA_BUFFER_OK;
}


PACKET_STATUS messageGet(buffer_object_t **packet){
    if(bufferGetData(&buffer,(void**)packet,1)!=BUFFER_STATUS_OK){
        return DATA_BUFFER_EMPTY;
    }
    return DATA_BUFFER_OK;
}

void messageAlloc(void **pt){

}

void messageFree(void **pt){
    messagePacket_t *packet = (messagePacket_t*)*pt;
    memset(packet->msg,0x00,packet->size); //Will write zeros to the message memory
    free(packet->msg); //Will free the message from the packet
    packet->size = 0;   //Will zero the size
    free(packet);    //Will free the packet
}


/*This is a basic test that illustrates the ability of the linked list to operate
 * as a FIFO buffer with dynamically changing memory usage. It provides the mechanisms
 * to check if data have been succesfully written/read or if there was an overflow.*/
int main(void){
    char tmpMessage[10]={0};
	printf("%s",stringWelcome);
	messagePacket_t *messagePacket;
	buffer_object_t *packet;
	if(bufferCreate(&buffer,BUFFER_TYPE_LINKED,sizeof(messagePacket_t),LIST_SIZE,&messageAlloc,&messageFree,NULL)!=BUFFER_STATUS_OK)
	{
	    printf("%s",stringFailedToInitialiseBuffer);
	    fflush(stdout);
	    return EXIT_FAILURE;
	}
	/*Memory active, do something with it*/

	/*Start adding messages*/

	for(int j=0;j<LOOP_RUNS;j++)
	{
        /*Try to overflow the list*/
        for(int k=0;k<TEST_OVERFLOW_ADD;k++)
        {
            sprintf(tmpMessage,"%s%d",testMessage1,k);
            if(messageAdd(tmpMessage)!=DATA_BUFFER_OK){
                printf("%s",stringFailedToAddPacket);
                fflush(stdout);
            }
        }
        /*Read some object from the list*/
        for(int k=0;k<TEST_OVERFLOW_GET;k++)
        {
            if(messageGet(&packet)!=DATA_BUFFER_OK){
                printf("%s",stringFailedToGetPacket);
                fflush(stdout);
            }
            else{
                messagePacket = (messagePacket_t*)packet->data;
                printf("%s\n",messagePacket->msg);
                packet->myFree((void**)&messagePacket);
                free(packet);
                fflush(stdout);

            }
        }
	}

	/*Check how many elements are still in the list and read them in order
	 * to clear the buffer and free all the heap. Repetitive code, can improve.*/
	int elements;
	bufferCountElements(&buffer,&elements);
	if(elements>0)
	{
	    printf("%s",stringElementsAvailable);
	    fflush(stdout);
	}
	while(elements>0){
        if(messageGet(&packet)!=DATA_BUFFER_OK){
            printf("%s",stringFailedToGetPacket);
            fflush(stdout);
        }
        else{
            messagePacket = (messagePacket_t*)packet->data;
            printf("%s\n",messagePacket->msg);
            packet->myFree((void**)&messagePacket);
            free(packet);
            fflush(stdout);

        }
        elements--;
	}
	return EXIT_SUCCESS;
}
