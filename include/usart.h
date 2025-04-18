#ifndef __USART_H
#define __USART_H

#include "stdint.h"
#include "stm32f103x6.h"
/*
    stm32f103Cx - 3 uart
*/

#define BRR_CALC(freq, baud)    ((freq)/(baud))

#define UART1_ENABLE
#define UART2_ENABLE
#define UART3_ENABLE
// #define UART4_ENABLE

#ifdef UART1_ENABLE

#define UART1_USE_RING_BUFF
#define UART1_TXBUFF_LENGHT     16
#define UART1_RXBUFF_LENGHT     16

#endif

#ifdef UART2_ENABLE

#define UART2_USE_RING_BUFF
#define UART2_TXBUFF_LENGHT     16
#define UART2_RXBUFF_LENGHT     16

#endif


#ifdef UART3_ENABLE

#define UART3_USE_RING_BUFF
#define UART3_TXBUFF_LENGHT     16
#define UART3_RXBUFF_LENGHT     16

#endif

#ifdef UART4_ENABLE

#define UART4_USE_RING_BUFF
#define UART4_TXBUFF_LENGHT     16
#define UART4_RXBUFF_LENGHT     16

#endif

typedef struct
{
    /* data */
    uint32_t bus_freq;
    uint32_t baud;
    uint8_t data_bits;
    uint8_t stop_bits;
    uint8_t parity;
} UARTInitStruct_t;


/** 
    @brief Инициализация UART
    @param id - номер порта
    @param init  
*/
uint8_t UART_Init(uint8_t id, const UARTInitStruct_t * init);

uint8_t UART_PutC(uint8_t id, const uint8_t c);

int16_t UART_GetC(uint8_t id);

//Получить количество непрочитанных байт в буфере приемника
uint8_t UART_BytesToRead(uint8_t id);
//Получить количество еще не отправленных байт в буфере передатчика
uint8_t UART_BytesToWrite(uint8_t id);
//Очистить буфер приемника
void UART_ReadBuffClear(uint8_t id);
//Очистить буфер передатчика
void UART_WriteBuffClear(uint8_t id);

#endif