#include "USART.h"

void Init_USART(u32 bound)
{
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ENABLE USART1
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ENABLE USART2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ENABLE USART3

    USART_DeInit(USART1); //复位串口1

    //USART
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //INIT USART1
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_Cmd(USART1, ENABLE);

    //INIT USART2
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);

    //INIT USART3
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);

    CLR_Usart3_Buf(); //clear buf
    CLR_Usart2_Buf();
}

void CLR_Usart3_Buf(void) //串口缓存清理
{
    memset(USART3_RX_BUF, 0, USART3_REC_LEN); //Clear
    Usart3_Point = 0;
}

void CLR_Usart2_Buf(void) //串口缓存清理
{
    memset(USART2_RX_BUF, 0, USART2_REC_LEN); //Clear
    Usart2_Point = 0;
}

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART2->SR & 0X40) == 0)
        ; //循环发送,直到发送完毕
    USART2->DR = (u8)ch;
    return ch;
}
#endif

void USART2_SendString(char *str)
{
    while (*str)
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
            ;
        USART_SendData(USART2, *str++);
    }
}

void USART2_Send_Command(char *str)
{
    CLR_Usart2_Buf();
    USART2_SendString(str);
    USART2_SendString("\r\n");
}

u8 USART2_Send_AT_Command(char *str, char *ret, u8 wait_time, u16 interval_time)
{
    u8 i;
    i = 0;
    while (i < wait_time)
    {
        USART2_Send_Command(str);
        for (i = 0; i < 10; i++)
        {
            delay_ms(interval_time);
        }
        if (Find(ret))
        {
            return true;
        }
        i++;
    }
    return false;
}

void USART2_Send_Command_END(char *str)
{
    CLR_Usart2_Buf();
    USART2_SendString(str);
}

u8 USART2_Send_AT_Command_End(char *str, char *ret, u8 wait_time, u32 interval_time)
{
    u8 i;
    i = 0;
    while (i < wait_time)
    {
        USART2_Send_Command_END(str);
        delay_ms(interval_time);
        if (Find(ret))
        {
            return true;
        }
        i++;
    }
    return false;
}
