#include "mylib.h"

// unsigned long mid_latitude_degree; //��γ�ȴ��� ���ȵĶ�
// unsigned long mid_latitude_points; //��γ�ȴ��� ���ȵķ�
// unsigned long mid_latitude_vale;   //��γ�ȴ��� ���ȵ��м����

// unsigned long mid_longitude_degree; //��γ�ȴ��� ���ȵĶ�
// unsigned long mid_longitude_points; //��γ�ȴ��� ���ȵķ�
// unsigned long mid_longitude_vale;   //��γ�ȴ��� ���ȵ��м����

static u8 fac_us = 0;  //us��ʱ������
static u16 fac_ms = 0; //ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()
{

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //ѡ���ⲿʱ��  HCLK/8
    fac_us = SystemCoreClock / 8000000;                   //Ϊϵͳʱ�ӵ�1/8
    fac_ms = (u16)fac_us * 1000;                          //��OS��,����ÿ��ms��Ҫ��systickʱ����
}
//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us;             //ʱ�����
    SysTick->VAL = 0x00;                      //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //�رռ�����
    SysTick->VAL = 0X00;                            //��ռ�����
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms;        //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;                      //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;      //�رռ�����
    SysTick->VAL = 0X00;                            //��ռ�����
}
//��ʱ S
//���Ϊ18s
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
//     mid_latitude_points = mid_latitude_points / 60;                //���뻻��ΪС��λ
//     mid_latitude_vale = mid_latitude_degree + mid_latitude_points; //����Ϊ�ȸ�ʽ000.00000000 �Ƕȷ����ʽ
//     Lin0_No[0] = '0';
//     Lin0_No[1] = mid_latitude_vale / 10000000 + 0x30; //ת��Ϊ�ַ�
//     Lin0_No[2] = (mid_latitude_vale / 1000000) % 10 + 0x30;
//     Lin0_No[3] = '.';
//     Lin0_No[4] = (mid_latitude_vale / 100000) % 10 + 0x30;
//     Lin0_No[5] = (mid_latitude_vale / 10000) % 10 + 0x30;
//     Lin0_No[6] = (mid_latitude_vale / 1000) % 10 + 0x30;
//     Lin0_No[7] = (mid_latitude_vale / 100) % 10 + 0x30;
//     Lin0_No[8] = (mid_latitude_vale / 10) % 10 + 0x30;
//     Lin0_No[9] = mid_latitude_vale % 10 + 0x30;
// }
