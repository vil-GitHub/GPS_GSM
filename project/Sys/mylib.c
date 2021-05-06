#include "mylib.h"

// unsigned long mid_latitude_degree; //经纬度处理 经度的度
// unsigned long mid_latitude_points; //经纬度处理 经度的分
// unsigned long mid_latitude_vale;   //经纬度处理 经度的中间变量

// unsigned long mid_longitude_degree; //经纬度处理 经度的度
// unsigned long mid_longitude_points; //经纬度处理 经度的分
// unsigned long mid_longitude_vale;   //经纬度处理 经度的中间变量

static u8 fac_us = 0;  //us延时倍乘数
static u16 fac_ms = 0; //ms延时倍乘数,在ucos下,代表每个节拍的ms数
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000;                   //为系统时钟的1/8
    fac_ms = (u16)fac_us * 1000;                          //非OS下,代表每个ms需要的systick时钟数
}
//延时nus
//nus为要延时的us数.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us;             //时间加载
    SysTick->VAL = 0x00;                      //清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //关闭计数器
    SysTick->VAL = 0X00;                            //清空计数器
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;        //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;                      //清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //关闭计数器
    SysTick->VAL = 0X00;                            //清空计数器
}
//延时 S
//最大为18s
void delay_s(u8 s)
{
    delay_ms(s * 100); //1
    delay_ms(s * 100); //2
    delay_ms(s * 100); //3
    delay_ms(s * 100); //4
    delay_ms(s * 100); //5
    delay_ms(s * 100); //6
    delay_ms(s * 100); //7
    delay_ms(s * 100); //8
    delay_ms(s * 100); //9
    delay_ms(s * 100); //10
}

//Exchange num to usc
int Num_Ucs2Gbk_Exchange(char *str)
{
    char num_tmp_saving[64];
    char *num_point = str;
    char temp_str[50] = {0};

    int i = 0;

    u8 single_num = 0;

    for (i = 0; i < 10; i++)
    {
        single_num = (int)(*num_point++);
        memset(num_tmp_saving, 0, sizeof(num_tmp_saving));
        switch (single_num)
        {
        case 46:
            sprintf(num_tmp_saving, "%s", "002E");
            break;
        case 48:
            sprintf(num_tmp_saving, "%s", "0030");
            break;
        case 49:
            sprintf(num_tmp_saving, "%s", "0031");
            break;
        case 50:
            sprintf(num_tmp_saving, "%s", "0032");
            break;
        case 51:
            sprintf(num_tmp_saving, "%s", "0033");
            break;
        case 52:
            sprintf(num_tmp_saving, "%s", "0034");
            break;
        case 53:
            sprintf(num_tmp_saving, "%s", "0035");
            break;
        case 54:
            sprintf(num_tmp_saving, "%s", "0036");
            break;
        case 55:
            sprintf(num_tmp_saving, "%s", "0037");
            break;
        case 56:
            sprintf(num_tmp_saving, "%s", "0038");
            break;
        case 57:
            sprintf(num_tmp_saving, "%s", "0039");
            break;
        default:
            continue;
        }
        strcat(temp_str, num_tmp_saving);
    }
    memset(str, 0, 50);
    strcat(str, temp_str);
    return true;
}

u8 Find(char *str)
{
    if (strstr(USART2_RX_BUF, str) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Print error
void errorLog(int num)
{
    char TEMP_Error_Buffer[Gps2Gsm_Buffer_Length] = {0};
    sprintf(TEMP_Error_Buffer, "95198BEF662F0020%d000D000A", num);
    strcpy(SaveData.Gps2Gsm_Buffer, TEMP_Error_Buffer);
}

// void Deal_Latitude_Gps(char *str)
// {
//     mid_latitude_degree = (str[0] - 0x30) * 10000000 + (str[1] - 0x30) * 1000000;
//     mid_latitude_points = (str[2] - 0x30) * 10000000 + (str[3] - 0x30) * 1000000 +
//                           (str[4] - 0x30) * 100000 + (str[5] - 0x30) * 10000 +
//                           (str[6] - 0x30) * 1000 + (str[7] - 0x30) * 100;
//     mid_latitude_points = mid_latitude_points / 60;                //分秒换算为小数位
//     mid_latitude_vale = mid_latitude_degree + mid_latitude_points; //最终为度格式000.00000000 非度分秒格式
//     Lin0_No[0] = '0';
//     Lin0_No[1] = mid_latitude_vale / 10000000 + 0x30; //转化为字符
//     Lin0_No[2] = (mid_latitude_vale / 1000000) % 10 + 0x30;
//     Lin0_No[3] = '.';
//     Lin0_No[4] = (mid_latitude_vale / 100000) % 10 + 0x30;
//     Lin0_No[5] = (mid_latitude_vale / 10000) % 10 + 0x30;
//     Lin0_No[6] = (mid_latitude_vale / 1000) % 10 + 0x30;
//     Lin0_No[7] = (mid_latitude_vale / 100) % 10 + 0x30;
//     Lin0_No[8] = (mid_latitude_vale / 10) % 10 + 0x30;
//     Lin0_No[9] = mid_latitude_vale % 10 + 0x30;
// }
