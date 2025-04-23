#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include <stdint.h>
#include <string.h>

#define CHAR_SIZE (sizeof(char))

typedef enum {
    RINGBUFFER_OK,          // Успех
    RINGBUFFER_ERR_NULL,    // 
    RINGBUFFER_ERR_EMPTY,   //
    RINGBUFFER_ERR_FULL,     //
    RINGBUFFFER_PARAM_ERR
} RINGBUFFER_STATUS;

typedef struct {
	uint8_t *buf;            ///< Указатель на буфер
    volatile size_t tail; ///< Точка чтения [ячейка]
    volatile size_t head; ///< Точка записи [ячейка]
    volatile size_t size; ///< Размер буфера [ячейка]
    volatile size_t cell_size; ///< Размер одной ячейки [байт]
} RINGBUFFER_t;


// Инициализация буфера
RINGBUFFER_STATUS RingBuffer_Init (RINGBUFFER_t* rb, void *buf, uint16_t size, size_t cellsize);
// Очиста буфера
RINGBUFFER_STATUS RingBuffer_Clear(RINGBUFFER_t* rb);
// Считать количество доступных для чтения ячеек
RINGBUFFER_STATUS RingBuffer_Available(RINGBUFFER_t* rb, uint16_t* len);

/* Запись*/
// Записать 1 байт в буфер
RINGBUFFER_STATUS RingBuffer_BytePut(RINGBUFFER_t* rb, const uint8_t data);
// Записать 1 ячейку заданного размера в буфер
RINGBUFFER_STATUS RingBuffer_CellPut(RINGBUFFER_t* rb, const void* data);
// Записать произвольное количество данных в буфер
RINGBUFFER_STATUS RingBuffer_DataPut(RINGBUFFER_t* rb, const void *data, uint16_t len);

/* Чтение (Считывание и очистка) */
// Прочесть 1 байт
RINGBUFFER_STATUS RingBuf_ByteRead(RINGBUFFER_t* rb, uint8_t *data);
// Прочесть 1 ячейку заданного размера
RINGBUFFER_STATUS RingBuf_CellRead(RINGBUFFER_t* rb, void *data);
// Прочесть произвольное количество данных
RINGBUFFER_STATUS RingBuf_DataRead(RINGBUFFER_t* rb, void *data, uint16_t len);

/* Просмотр (Считывание без очистки) */
// Прочесть 1 байт
RINGBUFFER_STATUS RingBuf_ByteWatch(RINGBUFFER_t* rb, uint8_t *data);
// Прочесть 1 ячейку заданного размера
RINGBUFFER_STATUS RingBuf_CellWatch(RINGBUFFER_t* rb, void *data);
// Прочесть произвольное количество данных
RINGBUFFER_STATUS RingBuf_DataWatch(RINGBUFFER_t* rb, void *data, uint16_t len);

#endif