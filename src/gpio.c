#include "gpio.h"

void portInit (void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);

    GPIOC->CRH |= (0x02 << GPIO_CRH_MODE13_Pos) | (0x00 << GPIO_CRH_CNF13_Pos);
}

void portSetHigh (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    // GPIOC->BSRR = ( 1 << 13 );
    if(PinState != GPIO_PIN_RESET)
    {
        GPIOx->BSRR = (GPIO_Pin);
    }
    else
    {
        GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;
    }
}

void portSetLow (void) {
    GPIOC->BRR = (1 << 13);
}