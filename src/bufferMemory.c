#include "bufferMemory.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

BUFFER_STATUS bufferCreate(buffer_t *buffer,BUFFER_TYPE type,size_t elementSize,size_t maxSize,void *allocateFunction,void *freeFunction,void *errorFunction)
{
    
    switch(type)
    {
        case BUFFER_TYPE_SIMPLE:
            //Not functional yet
            /*
            buffer->bufferType = type;
            buffer->numberOfElements = 0;
            buffer->sizeOfElements = elementSize;
            buffer->start = calloc(maxSize,elementSize);
            buffer->end = buffer->start;
            buffer->maxSize = maxSize;
            return BUFFER_STATUS_OK;
            */
            break;
        case BUFFER_TYPE_LINKED:
            buffer->bufferType = type;
            buffer->numberOfElements = 0;
            buffer->sizeOfElements = elementSize;
            buffer->maxSize = maxSize;
            buffer->myAlloc = (void*)allocateFunction;
            buffer->myFree = (void*)freeFunction;
            return BUFFER_STATUS_OK;
            break;
        default:
            break;
    }
    return BUFFER_STATUS_ERROR;
}


BUFFER_STATUS bufferGetData(void *buffer,void **data,size_t amount)
{
    BUFFER_TYPE type;
    buffer_t *buf;
    buf = (buffer_t*)buffer;
    if(buf->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    type = buf->bufferType;
    switch(type)
    {
        case BUFFER_TYPE_SIMPLE:
            memcpy(data,buf->start,amount);
            break;
        case BUFFER_TYPE_LINKED:
            if(buf->numberOfElements==0)
            {
                return BUFFER_STATUS_EMPTY;
            }
            else
            {
                *data = buf->start;
                buf->start = buf->start->next;
                buf->numberOfElements--;
            }
            break;
        default:
            break;
    }
    return BUFFER_STATUS_OK; 
}

BUFFER_STATUS bufferGetStatus(void *buffer)
{
    buffer_t *pt;
    pt = buffer;
    if(pt->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    if(pt->numberOfElements==0)return BUFFER_STATUS_EMPTY;
    if((pt->numberOfElements>0)&&(pt->numberOfElements<pt->maxSize))return BUFFER_STATUS_OK;
    if(pt->numberOfElements==pt->maxSize)return BUFFER_STATUS_FULL;
    return BUFFER_STATUS_OK;
}

BUFFER_STATUS bufferPopData(void *buffer,void **data,int index)
{
    BUFFER_TYPE type;
    buffer_t *buf;
    int k=0;
    buffer_object_t *packet;
    buf = (buffer_t*)buffer;
    if(buf->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    type = buf->bufferType;
    switch(type)
    {
        case BUFFER_TYPE_LINKED:
            if(buf->numberOfElements==0){
                    return BUFFER_STATUS_EMPTY;
            }
            else{
                if(index==0){
                    /*It is the first in queue so just move the entry to the second*/
                    packet = buf->start;
                    if(buf->numberOfElements>1){
                        buf->start = buf->start->next;
                    }
                    else{
                        buf->start = NULL;
                    }
                    
                    *data = packet;
                }
                else if(index == buf->numberOfElements -1){
                    packet = buf->end;
                    /*It is the last one in queue so just move the exit to the second from last*/
                    buf->end = buf->end->previous;
                    *data = packet;
                }
                else{
                    /*Now a packet has to be removed from the middle, so carefully join the 2 adjacent ones */
                    packet = buf->start;
                    for(k=1;k<=index;k++){
                        packet = packet->next;
                    }
                    packet->previous->next = packet->next;
                    packet->next->previous = packet->previous;
                    *data = packet;
                    
                }
                 buf->numberOfElements--;
            }
            return BUFFER_STATUS_OK;
        break;
        default: 
            break;
            
    
    }
    return BUFFER_STATUS_EMPTY;
}

BUFFER_STATUS bufferCountElements(void *buffer,int *count)
{
    BUFFER_TYPE type;
    buffer_t *buf;
    buf = (buffer_t*)buffer;
    if(buf->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    type = buf->bufferType;
    switch(type)
    {
        case BUFFER_TYPE_LINKED:
            *count = buf->numberOfElements;
            return BUFFER_STATUS_OK;
//            if(buf->numberOfElements==0){
//                
//                    return BUFFER_STATUS_EMPTY;
//            }
//            else if(index<buf->numberOfElements==0){
//                return BUFFER_STATUS_ERROR;
//            }
//            else{
//                packet = buf->start;
//                for(k=1;k<index;k++){
//                    packet = packet->next;
//                }
//                *data = packet;

//            }
        default:
            break;
    }
    return BUFFER_STATUS_ERROR;
}

BUFFER_STATUS bufferPeekData(void *buffer,void **data,int index)
{
    BUFFER_TYPE type;
    buffer_t *buf;
    int k=0;
    buffer_object_t *packet;
    buf = (buffer_t*)buffer;
    if(buf->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    type = buf->bufferType;
    switch(type)
    {
        case BUFFER_TYPE_LINKED:
            if(buf->numberOfElements==0){
                    return BUFFER_STATUS_EMPTY;
            }

            else if(index>buf->numberOfElements-1){
                return BUFFER_STATUS_ERROR;
            }

            else{
                packet = buf->start;
                for(k=1;k<=index;k++){
                    packet = packet->next;
                }
                *data = packet;
                return BUFFER_STATUS_OK;
            }
        default:
            return BUFFER_STATUS_ERROR;
            break;
    }
}
BUFFER_STATUS bufferAddData(void *buffer,void *data,size_t amount)
{
    BUFFER_TYPE type;
    buffer_t *buf;
    /*First create a packet*/
    buffer_object_t *bufferPacket;
    buf = (buffer_t*)buffer;
    if(buf->bufferType == BUFFER_TYPE_INACTIVE)return BUFFER_STATUS_INACTIVE;
    type = buf->bufferType;
    if(buf->numberOfElements>buf->maxSize)return BUFFER_STATUS_FULL;
    switch(type)
    {
        case BUFFER_TYPE_SIMPLE:
            memcpy(buf->start,data,amount);
            break;
        case BUFFER_TYPE_LINKED:
            /*Is there space for the packet*/
            if((buf->numberOfElements>=buf->maxSize))
                return BUFFER_STATUS_FULL;
            /*Allocate memory for the packet*/
            bufferPacket = (buffer_object_t*)malloc(sizeof(buffer_object_t));
            if(bufferPacket==NULL)return BUFFER_STATUS_ERROR; 
            bufferPacket->data = data;
            bufferPacket->myAlloc = buf->myAlloc;
            bufferPacket->myFree = buf->myFree;
            bufferPacket->next = NULL;
            bufferPacket->previous = NULL;
            /*Save the data*/
            if(buf->numberOfElements==0)
            {
                buf->end = bufferPacket;                
                buf->start = buf->end;
                buf->numberOfElements++;
                return BUFFER_STATUS_OK;
            }
            else if(buf->numberOfElements<buf->maxSize)
            {
                bufferPacket->previous = buf->end;
                buf->end->next = bufferPacket;
                buf->end = buf->end->next;
                buf->end->next = NULL;
                buf->numberOfElements++;
                return BUFFER_STATUS_OK;
            }
            else
            {
                /*Buffer full, should never arrive here*/
                return BUFFER_STATUS_FULL;
            }
            break;
        default:
            break;
    }
    return BUFFER_STATUS_ERROR;    
    
}

