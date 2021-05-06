#ifndef _MYLIB_H_
#define _MYLIB_H_
#include "USART.h"
#include "GPS.h"
#include "sys.h"

#define false 0
#define true 1

//GPS ERROR
#define GPS_SUBSTRING_ERROR 2
#define GPS_SUBSTRINGNEXT_ERROR 3

//GSM ERROR
#define GSM_COMMUNITE_ERROR -1
#define GSM_NO_SIM_CARD_ERROR -2
#define GSM_SIM_CARD_NO_REG_ERROR -3
#define GSM_MESSAGE_MODE_SET_ERROR -4
#define GSM_SET_MESSAGE_TEXT_MODE_ERROR -5
#define GSM_MESSAGE_SET_USC2_ERROR -6
#define GSM_CHINESE_MESSAGE_ERROR -7
#define GSM_AT_CMGS_ERROR -8
#define GSM_END_CHAR_ERROR -9

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_s(u8 s);
int Num_Ucs2Gbk_Exchange(char *str);
void errorLog(int num);
u8 Find(char *str);

#endif
