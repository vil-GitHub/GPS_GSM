#include "GPIO.h"

void Init_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    //USART1_TX PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Usart1_Tx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOGroup_Usart1, &GPIO_InitStructure);

    //USART1_RX PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Usart1_Rx;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOGroup_Usart1, &GPIO_InitStructure);

    //USART2_TX PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Usart2_Tx;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOGroup_Usart2, &GPIO_InitStructure);

    //USART2_RX PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Usart2_Rx;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOGroup_Usart2, &GPIO_InitStructure);

    //USART3_RX PB11
    GPIO_InitStructure.GPIO_Pin = GPIO_Usart3_RX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOGroup_Usart3, &GPIO_InitStructure);

    //KEY
    GPIO_InitStructure.GPIO_Pin = GPIO_Key;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_Init(GPIOGroup_Key, &GPIO_InitStructure);

    //LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Led;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_Init(GPIOGroup_Led, &GPIO_InitStructure);
}
