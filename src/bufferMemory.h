/* ************************************************************************** */
/** Descriptive File Name

  @Company
    vsel

  @Author
    santinio
  
  @File Name
    bufferMemory.h

 */
/* ************************************************************************** */

#ifndef _BUFFERMEMORY_H    /* Guard against multiple inclusion */
#define _BUFFERMEMORY_H
#include "stddef.h"

typedef enum{
    BUFFER_TYPE_INACTIVE=0,
    BUFFER_TYPE_SIMPLE,
    BUFFER_TYPE_LINKED
}BUFFER_TYPE;

typedef enum{
    BUFFER_STATUS_ERROR = -1,
    BUFFER_STATUS_INACTIVE,
    BUFFER_STATUS_OK,
    BUFFER_STATUS_FULL,
    BUFFER_STATUS_EMPTY,
}BUFFER_STATUS;

struct buffer_object
{
    struct buffer_object *next;
    struct buffer_object *previous;
    void *data;
    void * (*myFree) (void**);
    void * (*myAlloc) (void**, size_t);
};

typedef struct buffer_object buffer_object_t;

typedef struct buffer_tt{
    BUFFER_TYPE bufferType;
    buffer_object_t *start;
    buffer_object_t *end;
    size_t numberOfElements;
    size_t sizeOfElements;
    size_t maxSize;
    void * (*myFree) (void**);
    void * (*myAlloc) (void**, size_t);
}buffer_t;

//typedef struct buffer_tt buffer_t;
/*Pointer to the buffer, type of buffer, size of each element, max size, allocate, free and error function
 Not sure if I need number of elements and max size, should be the same.*/

/*Pass the pointer of the list and a pointer for an integer to write the amount
 *  of objects in the list*/
BUFFER_STATUS bufferCountElements(void*,int*);
/*Pass the pointer of the list, a double-pointer to a buffer_object and the index.
 *  It will point the pointer to the object at the given index. It will remove the
 *  object from the list.*/
BUFFER_STATUS bufferPopData(void*,void**,int);
/*Pass the pointer of the list, a double-pointer to a buffer_object and the index.
 *  It will point the pointer to the object at the given index. It will NOT remove
 *   the object from the list.*/
BUFFER_STATUS bufferPeekData(void*,void**,int);
/*Pass the pointer of the list, the list type, the size of each object, the maximum
 * allowed objects, a pointer to the allocate function, a pointer to the free function
 * and a pointer to the error function */
BUFFER_STATUS bufferCreate(buffer_t*,BUFFER_TYPE,size_t,size_t,void*,void*,void*);
/*Pass the pointer of the list, a double-pointer to a buffer_object and the amount
 * of objects that should be read. ONLY 1 works*. The data will be read from the
 * beginning of the list.*/
BUFFER_STATUS bufferGetData(void*,void**,size_t);
/*Pass the pointer of the list, a double-pointer to a buffer_object and the amount
 * of objects that should be written. ONLY 1 works*. The data will be written to the
 * end of the list.*/
BUFFER_STATUS bufferAddData(void*,void*,size_t);
/*Pass the pointer of the list, it will return the status of the current list*/
BUFFER_STATUS bufferGetStatus(void*);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
