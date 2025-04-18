#include "rcc.h"

int clockInit (void) {
    RCC->CR |= ( 1 << RCC_CR_HSEON_Pos);

    __IO int StartUpCounter;
    //Ждем успешного запуска или окончания тайм-аута
    for(StartUpCounter=0; ; StartUpCounter++)
    {
      //Если успешно запустилось, то 
      //выходим из цикла
      if(RCC->CR & (1<<RCC_CR_HSERDY_Pos))
        break;
      
      //Если не запустилось, то
      //отключаем все, что включили
      //и возвращаем ошибку
      if(StartUpCounter > 0x1000)
      {
        RCC->CR &= ~(1<<RCC_CR_HSEON_Pos); //Останавливаем HSE
        return 1;
      }
    }

    RCC->CFGR |= (0x07 << RCC_CFGR_PLLMULL_Pos)
              |  (0x01 << RCC_CFGR_PLLSRC_Pos);
    
    RCC->CR |= (1<<RCC_CR_PLLON_Pos);

      //Ждем успешного запуска или окончания тайм-аута
    for(StartUpCounter=0; ; StartUpCounter++)
    {
        //Если успешно запустилось, то 
        //выходим из цикла
        if(RCC->CR & (1<<RCC_CR_PLLRDY_Pos))
        break;
        
        //Если по каким-то причинам не запустился PLL, то
        //отключаем все, что включили
        //и возвращаем ошибку
        if(StartUpCounter > 0x1000)
        {
        RCC->CR &= ~(1<<RCC_CR_HSEON_Pos); //Останавливаем HSE
        RCC->CR &= ~(1<<RCC_CR_PLLON_Pos); //Останавливаем PLL
        return 2;
        }
    }

    ////////////////////////////////////////////////////////////
    //Настраиваем FLASH и делители
    ////////////////////////////////////////////////////////////

    //Устанавливаем 2 цикла ожидания для Flash
    //так как частота ядра у нас будет 48 MHz < SYSCLK <= 72 MHz
    FLASH->ACR |= (0x02<<FLASH_ACR_LATENCY_Pos);

    //Делители
    RCC->CFGR |= (0x00 << RCC_CFGR_PPRE2_Pos)   // Делитель шины APB2 отключен
                | (0x04<<RCC_CFGR_PPRE1_Pos)    // Делитель нишы APB1 равен 2
                | (0x00<<RCC_CFGR_HPRE_Pos);    // Делитель AHB отключен

    RCC->CFGR |= (0x02 << RCC_CFGR_SW_Pos);

    //Ждем, пока переключимся
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x02<<RCC_CFGR_SWS_Pos))
    {
    }
    
    //После того, как переключились на
    //внешний источник такирования
    //отключаем внутренний RC-генератор
    //для экономии энергии
    RCC->CR &= ~(1<<RCC_CR_HSION_Pos);
    
    //Настройка и переклбючение сисемы
    //на внешний кварцевый генератор
    //и PLL запершилось успехом.
    //Выходим
    
    return 0;
}