#ifndef _GSM_H_
#define _GSM_H_
#include "USART.h"
#include "mylib.h"

#define PHONE_NUM "13083830861" //目标电话号码

extern u16 Usart2_Point;

int Send_Chinese_Message(char *message);
void Init_AT_Command(void);

#endif
