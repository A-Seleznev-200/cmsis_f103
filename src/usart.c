#include "usart.h"
#include "RingBuffer.h"
/******************************************************************************/


#define RXNE(UARTx)     (UARTx->SR & USART_SR_RXNE)
#define TXE(UARTx)      (UARTx->SR & USART_SR_TXE)

#define RXNEIE(UARTx)   (UARTx->CR1 & USART_CR1_RXNEIE)

static void TxPinInit(__IO uint32_t *CRx, 
    uint32_t GPIO_CRL_MODEx_Pos, 
    uint32_t GPIO_CRL_CNFx_Pos);


static void RxPinInit(__IO uint32_t *CRx, 
    __IO uint32_t *BSRR,
    uint32_t GPIO_CRL_MODEx_Pos, 
    uint32_t GPIO_CRL_CNFx_Pos,
    uint32_t PinN);

static void _uart_en(USART_TypeDef *UARTx);

static int16_t _uart_init(USART_TypeDef *UARTx, const UARTInitStruct_t *init);
static void TXEIEnable(USART_TypeDef *UARTx);
static void TXEIDisable(USART_TypeDef *UARTx);
/******************************************************************************/

/** 
    @brief Инициализация UART
    @param id - номер порта
    @param init  

    @return 0 - успех, 1 - ошибка инициализации
*/
uint8_t UART_Init(uint8_t id, const UARTInitStruct_t *init)
{
#ifdef UART1_ENABLE
  /*
  ПОРТЫ:
        DEFAULT  REMAP
  TX1     PA9     PB6
  RX1     PA10    PB7
  
  ШИНА:
  APB2
  */
    if (id == 1)
    {
        // тактирование модуля uart
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

        // настройка портов
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

        // настройка TX (PA9)
        TxPinInit(&GPIOA->CRH,
                    GPIO_CRH_MODE9_Pos,
                    GPIO_CRH_CNF9_Pos);

        // настройка RX (PA10)
        RxPinInit(&GPIOA->CRH, 
            &GPIOA->BSRR, 
            GPIO_CRH_MODE10_Pos, 
            GPIO_CRH_CNF10_Pos, 
            10);

            //Сброс модуля
        RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
        asm("nop");
        asm("nop");
        asm("nop");
        RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;
        asm("nop");
        asm("nop");
        asm("nop");

        //Инициализация основных регистров UART
        if(_uart_init(USART1, init) < 0)
        {
            return -1;
        }

        _uart_en(USART1);

        return 0;
    }
#endif
    
    if (id == 2)
    {
        return 0;
    }

    if (id == 3)
    {
        return 0;
    }

    if (id == 4)
    {
        return 0;
    }
    return 1;
}

/** 
    @brief Записать символ в буфер передатчика
    @param id - номер порта
    @param с - отправляемый символ  

    @return 0 - успех, 1 - ошибка, буфер переполнен
*/
uint8_t UART_PutC(uint8_t id, const uint8_t c)
{
    switch (id)
    {
//////////////////////////////////////////////////////
#ifdef UART1_ENABLE
    case 1:
#ifdef UART1_USE_RINGBUFF
    return RingBuffer_BytePut(rb, c);
#else
    if (TXE(USART1))
    {
        USART1->DR = c;
        return c;
    }
    else { return -1; }
#endif
        break;
#endif
//////////////////////////////////////////////////////
// #ifdef UART2_ENABLE
//     case 2;
// #ifdef UART2_USE_RING_BUFF
//         return return RingBuffer_BytePut(rb, c);
// #else
    default:
        break;
    }
    return 0;
}

/** 
    @brief Прочитать символ из буфера приемника
    @param id - номер порта  

    @return -1 - нет данных для чтения, 0..255 - прочитанный символ
*/
int16_t UART_GetC(uint8_t id)
{
    return 0;
}

/** 
    @brief Получить количество непрочитанных байт в буфере приемника
    @param id - номер порта

    @return 0..255
*/
uint8_t UART_BytesToRead(uint8_t id)
{
    return 0;
}

/** 
    @brief Получить количество еще не отправленных байт в буфере передатчика
    @param id - номер порта

    @return количество еще не отправленных байт
*/
uint8_t UART_BytesToWrite(uint8_t id)
{
    return 0;
}

/** 
    @brief Очистить буфер приемника
    @param id - номер порта
*/
void UART_ReadBuffClear(uint8_t id)
{
}

/** 
    @brief Очистить буфер передатчика
    @param id - номер порта
*/
void UART_WriteBuffClear(uint8_t id)
{
}


/******************************************************************************/



static void TxPinInit(__IO uint32_t *CRx, 
    uint32_t GPIO_CRL_MODEx_Pos, 
    uint32_t GPIO_CRL_CNFx_Pos)
{
*CRx &= ~((0x03 << GPIO_CRL_MODEx_Pos) | (0x03 << GPIO_CRL_CNFx_Pos));
*CRx |= (0x01 << GPIO_CRL_MODEx_Pos) //01: Output mode, max speed 10 MHz.
| (0x02 << GPIO_CRL_CNFx_Pos); //10: Alternate function output Push-pull
}


static void RxPinInit(__IO uint32_t *CRx, 
    __IO uint32_t *BSRR,
    uint32_t GPIO_CRL_MODEx_Pos, 
    uint32_t GPIO_CRL_CNFx_Pos,
    uint32_t PinN)
{
*CRx &= ~((0x03 << GPIO_CRL_MODEx_Pos) | (0x03 << GPIO_CRL_CNFx_Pos));
*CRx |= (0x00 << GPIO_CRL_MODEx_Pos) //00: Input mode (reset state)
| (0x02 << GPIO_CRL_CNFx_Pos); //10: Input with pull-up / pull-down
*BSRR = 1 << PinN; //pull-up
}


/******************************************************************************/

static int16_t _uart_init(USART_TypeDef *UARTx, const UARTInitStruct_t *init)
{
  uint32_t UARTx_CR1_VAL = 0;
  
  
  //установка скорости UART
  UARTx->BRR = (uint16_t)BRR_CALC(init->bus_freq, init->baud);
  
  
  UARTx_CR1_VAL |= USART_CR1_TE | USART_CR1_RE;
  
  ///////////////////////////////////////
  
  switch(init->data_bits)
  {
  case 8:
    break;
   
  case 9:
    UARTx_CR1_VAL |= USART_CR1_M;
    break;
    
  default:
   return -1;
  }
  
  
  ///////////////////////////////////////
  
  switch(init->parity)
  {
  case 0: //None
    break;
    
  case 1: //Even
    UARTx_CR1_VAL |= USART_CR1_PCE;
    break;
    
  case 2: //Odd
    UARTx_CR1_VAL |= USART_CR1_PCE | USART_CR1_PS;
    break;
    
  default:
   return -1;
  }
  
  UARTx->CR1 = UARTx_CR1_VAL;
  
  ///////////////////////////////////////
  
  switch(init->stop_bits)
  {
  case 1:
    break;
    
  case 2:
    UARTx->CR2 |= (0x02 << USART_CR2_STOP_Pos);
    break;
    
  default:
   return -1;
  }
  
  
  return 0;
}

static void _uart_en(USART_TypeDef *UARTx)
{
  UARTx->CR1 |= USART_CR1_UE;
}

/******************************************************************************/


static void TXEIEnable(USART_TypeDef *UARTx)
{
  UARTx->CR1 |= USART_CR1_TXEIE;
}

static void TXEIDisable(USART_TypeDef *UARTx)
{
  UARTx->CR1 &= ~USART_CR1_TXEIE;
}

static int8_t ring_put (IRQn_Type IRQn, 
                        USART_TypeDef *UARTx, 
                        uint16_t BuffLen, 
                        RINGBUFFER_t *fifo, 
                        uint8_t c) 
{
    int16_t ret;

    NVIC_DisableIRQ(IRQn);
    if ((BuffLen - RingBuffer_Available(fifo, BuffLen)) > 0)
    {
        RingBuffer_BytePut(fifo, c);
        ret = c;
        TXEIEnable(UARTx);
    }
    else
    {
        ret = -1;
    }
    NVIC_EnableIRQ(IRQn);

    return ret;
    
}