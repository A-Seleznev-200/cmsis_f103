#ifndef __MAIN_H
#define __MAIN_H
#include <stdint.h>

uint16_t setBit(uint16_t number, uint16_t i);
int clockInit (void);

void init(void);
void delay(__IO uint32_t tck);


#endif