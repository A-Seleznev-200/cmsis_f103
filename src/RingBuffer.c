#include "RingBuffer.h"

RINGBUFFER_STATUS RingBuffer_Init(RINGBUFFER_t* rb, void *buf, uint16_t size, size_t cellsize)
{
    rb->buf = buf;              // set pointer to buffer
    rb->size = size;            // size of array
    rb->cell_size = cellsize;   // size of 1 cell of array
    RingBuffer_Clear(rb);       // clear all
    return rb->buf ? RINGBUFFER_OK : RINGBUFFFER_PARAM_ERR;
}

RINGBUFFER_STATUS RingBuffer_Clear(RINGBUFFER_t *rb)
{
    if (rb->buf == NULL)
    {
        return RINGBUFFER_OK;
    }
    rb->head = rb->tail = 0;
    
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuffer_Available(RINGBUFFER_t *rb, uint16_t *len)
{
    if (rb->buf == NULL)
    {
        return RINGBUFFFER_PARAM_ERR;
    }
    if (rb->head < rb->tail)
    {
        *len = rb->size - rb->tail + rb->head;
    }
    else
    {
        *len = rb->head - rb->tail;
    }
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuffer_BytePut(RINGBUFFER_t *rb, const uint8_t data)
{
    if (rb->buf == NULL)
    {
        return RINGBUFFFER_PARAM_ERR;
    }
    if (rb->head > rb->size)
    {
        rb->head = 0;   // set to start
    }
    
    rb->buf[rb->head] = data;
    rb->head++;
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuffer_CellPut(RINGBUFFER_t *rb, const void *data)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuffer_DataPut(RINGBUFFER_t *rb, const void *data, uint16_t len)
{
    if (rb->buf == NULL)
    {
        return RINGBUFFFER_PARAM_ERR;
    }
    if (len > rb->size)
    {
        return RINGBUFFER_ERR_FULL;
    }
    const char* input = data;
    
    for (uint16_t i = 0; i < len; i++)
    {
        RingBuffer_BytePut(rb, input[i]);
    }
    

    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_ByteRead(RINGBUFFER_t *rb, uint8_t *data)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_CellRead(RINGBUFFER_t *rb, void *data)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_DataRead(RINGBUFFER_t *rb, void *data, uint16_t len)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_ByteWatch(RINGBUFFER_t *rb, uint8_t *data)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_CellWatch(RINGBUFFER_t *rb, void *data)
{
    return RINGBUFFER_OK;
}

RINGBUFFER_STATUS RingBuf_DataWatch(RINGBUFFER_t *rb, void *data, uint16_t len)
{
    return RINGBUFFER_OK;
}
