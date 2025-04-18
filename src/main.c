#include "stm32f103x6.h"

#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "dwt.h"

#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC

static const UARTInitStruct_t UARTInitStr = 
{
  .bus_freq = 36000000,
  .baud = 115200,
  .data_bits = 8,
  .stop_bits = 1,
  .parity = 0,
};

uint16_t setBit(uint16_t number, uint16_t i) {
    return number |= (1 << i);
}

void delay(__IO uint32_t tck)
{
  while(tck)
  {
    tck--;
  }  
}

int main(void) {
  init();
  
  for(;;) {
    portSetHigh(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    DWT_Delay(1000000);
    portSetLow();
    DWT_Delay(1000000);
  }
}

void init(void) {
    clockInit();
    dwt_init();
    UART_Init(1, &UARTInitStr);
    portInit();
}

