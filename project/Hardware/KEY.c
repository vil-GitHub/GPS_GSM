#include "KEY.h"

int Key_Flag;

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        delay_ms(10);
        Key_Flag = true;
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}
