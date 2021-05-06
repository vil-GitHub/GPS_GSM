/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "GPIO.h"
#include "NVIC.h"
#include "USART.h"
#include "EXTI.h"
#include "GPS.h"
#include "GSM.h"
#include "KEY.h"
#include "LED.h"
#include "sys.h"
#include "mylib.h"

void Init(void)
{
  Init_GPIO();
  Init_NVIC();
  Init_EXTI();
  Init_USART(9600);
  Init_AT_Command();
  delay_init();
  Clr_Struct();
}

int main(void)
{
  int res;
  Init();
  Reset_Led();
  while (1)
  {
    if (Key_Flag == true)
    {
      Key_Flag = false;
      Reset_Led();
      delay_ms(200);
      Set_Led();
      ParseGpsBuffer();
      PackageGpsBuffer();
      res = Send_Chinese_Message(SaveData.Gps2Gsm_Buffer);
      if (res == true || res == GSM_END_CHAR_ERROR)
      {
        Reset_Led();
      }
      Clr_Struct();
    }
  }
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
