#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <stdint.h>

#define CHAR_SIZE (sizeof(char))

#define RINGBUFFER_OK (0)
#define RINGBUFFER_ERR_NULL (-1)
#define RINGBUFFER_ERR_EMPTY (-2)
#define RINGBUFFER_ERR_FULL (-3)

typedef struct {
	char* start;
	char* end;
	volatile char* readptr;
	volatile char* writeptr;
	volatile int full;
} RingBuffer;

void RingBuffer_Init (uint32_t capacity);
int8_t RingBuffer_Write(RingBuffer* rb, char c);
int8_t RingBuffer_Read(RingBuffer* rb, char c);
int8_t RingBuffer_Clear(RingBuffer* rb);

#endif