#ifndef _GPIO_H_
#define _GPIO_H_
#include "stm32f10x_gpio.h"

#define GPIOGroup_Usart1 GPIOA
#define GPIO_Usart1_Tx GPIO_Pin_9
#define GPIO_Usart1_Rx GPIO_Pin_10

#define GPIOGroup_Usart2 GPIOA
#define GPIO_Usart2_Tx GPIO_Pin_2
#define GPIO_Usart2_Rx GPIO_Pin_3

#define GPIOGroup_Usart3 GPIOB
#define GPIO_Usart3_RX GPIO_Pin_11

#define GPIOGroup_Key GPIOA
#define GPIO_Key GPIO_Pin_0

#define GPIOGroup_Led GPIOC
#define GPIO_Led GPIO_Pin_13

void Init_GPIO(void);

#endif
