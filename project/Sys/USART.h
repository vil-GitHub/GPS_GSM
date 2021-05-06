#ifndef _USART_H_
#define _USART_H_
#include <stdio.h>
#include <string.h>
#include "stm32f10x_usart.h"
#include "GPS.h"
#include "GSM.h"
#include "mylib.h"

#define USART3_REC_LEN 200 //USART1_BUF_MAX is 200bit
#define USART2_REC_LEN 100 //USART2_BUF_MAX is 100bit

extern char USART3_RX_BUF[USART3_REC_LEN];
extern char USART2_RX_BUF[USART2_REC_LEN];

void Init_USART(u32 bound);
void CLR_Usart3_Buf(void);
void CLR_Usart2_Buf(void);
u8 USART2_Send_AT_Command(char *str, char *ret, u8 wait_time, u16 interval_time);
u8 USART2_Send_AT_Command_End(char *str, char *ret, u8 wait_time, u32 interval_time);
void USART2_Send_Command(char *str);
void USART2_SendString(char *str);

#endif
