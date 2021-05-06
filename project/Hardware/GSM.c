#include "GSM.h"

char *Phone_num = PHONE_NUM;
char *ATD_pointer = NULL;

char text_message_Chinese[64];
char AT_command_saving[32];

char USART2_RX_BUF[USART2_REC_LEN];
u16 Usart2_Point = 0;

void USART2_IRQHandler(void)
{
    u8 Res;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART2);
        USART2_RX_BUF[Usart2_Point++] = Res;
        if (Usart2_Point > USART2_REC_LEN)
        {
            Usart2_Point = 0;
        }
    }
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART2);
    }
}

//Exchange num to usc
int Phone_Num_Ucs2Gbk_Exchange(void)
{
    char phone_num_tmp_saving[64];

    int i = 0;
    int phone_length = 0;
    u8 single_num = 0;
    phone_length = strlen(Phone_num);

    for (i = 0; i < phone_length; i++)
    {
        single_num = (int)(*Phone_num++);
        memset(phone_num_tmp_saving, 0, sizeof(phone_num_tmp_saving));
        switch (single_num)
        {
        case 48:
            sprintf(phone_num_tmp_saving, "%s", "0030");
            break;
        case 49:
            sprintf(phone_num_tmp_saving, "%s", "0031");
            break;
        case 50:
            sprintf(phone_num_tmp_saving, "%s", "0032");
            break;
        case 51:
            sprintf(phone_num_tmp_saving, "%s", "0033");
            break;
        case 52:
            sprintf(phone_num_tmp_saving, "%s", "0034");
            break;
        case 53:
            sprintf(phone_num_tmp_saving, "%s", "0035");
            break;
        case 54:
            sprintf(phone_num_tmp_saving, "%s", "0036");
            break;
        case 55:
            sprintf(phone_num_tmp_saving, "%s", "0037");
            break;
        case 56:
            sprintf(phone_num_tmp_saving, "%s", "0038");
            break;
        case 57:
            sprintf(phone_num_tmp_saving, "%s", "0039");
            break;
            // default:
            //     printf("出错了\r\n");
            //     return false;
        }
        strcat(AT_command_saving, phone_num_tmp_saving);
    }
    return true;
}

//Package phone_num
void Sim800C_AT_command_expend(char char1[], char char2[])
{
    char TEMP_AT_command_saving[32];

    memset(AT_command_saving, 0, sizeof(AT_command_saving));
    memset(TEMP_AT_command_saving, 0, sizeof(TEMP_AT_command_saving));
    sprintf(TEMP_AT_command_saving, "%s", char1);
    strcat(AT_command_saving, TEMP_AT_command_saving);

    Phone_Num_Ucs2Gbk_Exchange();

    memset(TEMP_AT_command_saving, 0, sizeof(TEMP_AT_command_saving));
    sprintf(TEMP_AT_command_saving, "%s", char2);
    strcat(AT_command_saving, TEMP_AT_command_saving);

    strcat(text_message_Chinese, AT_command_saving);
    memset(AT_command_saving, 0, sizeof(AT_command_saving));
}

//Init phone_number
void Init_AT_Command(void)
{
    Sim800C_AT_command_expend("AT+CMGS=\"", "\""); //例子：AT+CMGS="00310030003000380036"
}

u8 Wait_CREG(u8 query_times)
{
    u8 wait_flag, wait_count;
    wait_flag = true;
    wait_count = 0;
    CLR_Usart2_Buf();
    while (wait_flag)
    {
        USART2_Send_Command("AT+CREG?");
        for (int i = 0; i < 20; i++)
        {
            delay_ms(100);
        }
        for (int k = 0; k < USART2_REC_LEN; k++)
        {
            if ((USART2_RX_BUF[k] == '+') && (USART2_RX_BUF[k + 1] == 'C') && (USART2_RX_BUF[k + 2] == 'R') && (USART2_RX_BUF[k + 3] == 'E') && (USART2_RX_BUF[k + 4] == 'G') && (USART2_RX_BUF[k + 5] == ':'))
            {
                if ((USART2_RX_BUF[k + 7] == '0') && ((USART2_RX_BUF[k + 9] == '1') || (USART2_RX_BUF[k + 9] == '6')))
                {
                    wait_flag = false;
                    return true;
                }
            }
        }
        wait_count++;
        if (wait_count > query_times)
        {
            return false;
        }
    }
    return false;
}

//Package content
int Send_Chinese_Text_Message(char *content)
{
    char end_char[2];

    end_char[0] = 0x1A;
    end_char[1] = '\0';

    ATD_pointer = text_message_Chinese; //字符指针

    if (USART2_Send_AT_Command(ATD_pointer, ">", 3, 50) == false)
        return GSM_AT_CMGS_ERROR;
    USART2_SendString(content);
    USART2_Send_AT_Command_End(end_char, "+CMGS", 1, 350);

    return true;
}

//Package chinese message
int Send_Chinese_Message(char *message)
{
    if (USART2_Send_AT_Command("AT", "OK", 3, 50) == false)
        return GSM_COMMUNITE_ERROR;
    if (USART2_Send_AT_Command("AT+CPIN?", "READY", 2, 50) == false)
        return GSM_NO_SIM_CARD_ERROR;
    if (Wait_CREG(3) == false)
        return GSM_SIM_CARD_NO_REG_ERROR;
    if (USART2_Send_AT_Command("AT+CMGF=1", "OK", 2, 50) == false)
        return GSM_MESSAGE_MODE_SET_ERROR;
    if (USART2_Send_AT_Command("AT+CSMP=17,167,1,8", "OK", 2, 50) == false)
        return GSM_SET_MESSAGE_TEXT_MODE_ERROR;
    if (USART2_Send_AT_Command("AT+CSCS=\"UCS2\"", "OK", 2, 50) == false)
        return GSM_MESSAGE_SET_USC2_ERROR;
    if (Send_Chinese_Text_Message(message) == true)
        return true;

    return GSM_CHINESE_MESSAGE_ERROR;
}
