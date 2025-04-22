#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <stdint.h>
#include <string.h>

#define CHAR_SIZE (sizeof(char))

typedef enum {
    RINGBUFFER_OK,          // Успех
    RINGBUFFER_ERR_NULL,    // 
    RINGBUFFER_ERR_EMPTY,   //
    RINGBUFFER_ERR_FULL     //
} RINGBUFFER_STATUS;

typedef struct {
	uint8_t *buf;            ///< Указатель на буфер
    volatile size_t tail; ///< Точка чтения [ячейка]
    volatile size_t head; ///< Точка записи [ячейка]
    volatile size_t size; ///< Размер буфера [ячейка]
    volatile size_t cell_size; ///< Размер одной ячейки [байт]
} RINGBUFFER_t;


// Инициализация буфера
void RingBuffer_Init (uint32_t capacity);

int8_t RingBuffer_Write(RINGBUFFER_t* rb, char c);
int8_t RingBuffer_Read(RINGBUFFER_t* rb, char c);
int8_t RingBuffer_Clear(RINGBUFFER_t* rb);

#endif