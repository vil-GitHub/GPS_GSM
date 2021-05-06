#include "LED.h"

void Set_Led(void)
{
    GPIO_WriteBit(GPIOGroup_Led, GPIO_Led, Bit_RESET);
}

void Reset_Led(void)
{
    GPIO_WriteBit(GPIOGroup_Led, GPIO_Led, Bit_SET);
}
