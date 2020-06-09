/* Includes ------------------------------------------------------------------*/
#include <assert.h>
#include "ring_buffer.h"
#include <stdio.h>

void __aeabi_assert(const char *expr, const char *file, int line) { 
    printf("ASSERT ERROR in file %s at line %d.\nExpression: %s\nGoing into infinite loop.\n", file, line, expr);
    while(1) {
        ;
    }
}

bool RingBuffer_Init(RingBuffer *ringBuffer, char *dataBuffer, size_t dataBufferSize) 
{
	assert(ringBuffer);
	assert(dataBuffer);
	assert(dataBufferSize > 0);
	
	if ((ringBuffer) && (dataBuffer) && (dataBufferSize > 0)) {
	  
	  ringBuffer->ptr=dataBuffer;
	  ringBuffer->size=dataBufferSize;
	  ringBuffer->head=0;
	  ringBuffer->tail=0;
	  ringBuffer->count=0;
	  return true;
	}
	
	return false;
}

bool RingBuffer_Clear(RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
        ringBuffer->count  = 0;
        ringBuffer->head   = 0;
        ringBuffer->tail   = 0;
        return true;
	}
	return false;
}

bool RingBuffer_IsEmpty(const RingBuffer *ringBuffer)
{
  assert(ringBuffer);	
	
	if(ringBuffer->count==0)return true;
	return false;
	
	
}

size_t RingBuffer_GetLen(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->count;
	}
	return 0;
	
}

size_t RingBuffer_GetCapacity(const RingBuffer *ringBuffer)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		return ringBuffer->size;
	}
	return 0;	
}


bool RingBuffer_PutChar(RingBuffer *ringBuffer, char c)
{
	assert(ringBuffer);
	
	if (ringBuffer) {
		if (ringBuffer->count < ringBuffer->size)
        {
          ringBuffer->ptr[ringBuffer->head] = c;
          if(ringBuffer->head==ringBuffer->size-1) {
						ringBuffer->head=0;
					} else {
						ringBuffer->head++;
					}          
          ++ringBuffer->count;
          return true;
        }
	}
	return false;
}

bool RingBuffer_GetChar(RingBuffer *ringBuffer, char *c)
{
	assert(ringBuffer);
	assert(c);
	
  if ((ringBuffer) && (c)) {
    if (ringBuffer->count>0)
    {
      *c = ringBuffer->ptr[ringBuffer->tail];
       if(ringBuffer->tail==ringBuffer->size-1) {
				 ringBuffer->tail=0;
			 } else {
				 ringBuffer->tail++;
			 }
      --ringBuffer->count;
			return true;
    }		
	}
	return false;
}
