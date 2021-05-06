#include "GPS.h"

char USART3_RX_BUF[USART3_REC_LEN];
u16 Usart3_Point = 0;

SaveData_Typedef SaveData;

void USART3_IRQHandler(void)
{
    u8 Res;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        Res = USART_ReceiveData(USART3); //(USART3->DR);
        if (Res == '$')
        {
            Usart3_Point = 0;
        }
        USART3_RX_BUF[Usart3_Point++] = Res;
        if (USART3_RX_BUF[0] == '$' && USART3_RX_BUF[4] == 'M' && USART3_RX_BUF[5] == 'C') //确认是否收到"XXRMC"这一帧数据
        {
            if (Res == '\n')
            {
                memset(SaveData.GPS_Buffer, 0, GPS_Buffer_Length);        //Clear
                memcpy(SaveData.GPS_Buffer, USART3_RX_BUF, Usart3_Point); //Save data
                SaveData.isGetData = true;
                Usart3_Point = 0;
                memset(USART3_RX_BUF, 0, USART3_REC_LEN); //Clear
            }
        }
        if (Usart3_Point >= USART3_REC_LEN)
        {
            Usart3_Point = USART3_REC_LEN;
        }
    }
}

void Clr_Struct(void)
{
    memset(&SaveData, 0, sizeof(SaveData_Typedef));
}

//Parse gps message
void ParseGpsBuffer(void)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    if (SaveData.isGetData)
    {
        SaveData.isGetData = false;
        //printf("*************\r\n");
        //printf(SaveData.GPS_Buffer);

        for (i = 0; i <= 6; i++)
        {
            if (i == 0)
            {
                if ((subString = strstr(SaveData.GPS_Buffer, ",")) == NULL)
                {
                    errorLog(GPS_SUBSTRING_ERROR); //error
                }
            }
            else
            {
                subString++;
                if ((subStringNext = strstr(subString, ",")) != NULL)
                {
                    char usefullBuffer[2];
                    switch (i)
                    {
                    case 1:
                        memcpy(SaveData.UTCTime, subString, subStringNext - subString);
                        break;
                    case 2:
                        memcpy(usefullBuffer, subString, subStringNext - subString);
                        break;
                    case 3:
                        memcpy(SaveData.latitude, subString, subStringNext - subString);
                        break;
                    case 4:
                        memcpy(SaveData.N_S, subString, subStringNext - subString);
                        break;
                    case 5:
                        memcpy(SaveData.longitude, subString, subStringNext - subString);
                        break;
                    case 6:
                        memcpy(SaveData.E_W, subString, subStringNext - subString);
                        break;
                    default:
                        break;
                    }
                    subString = subStringNext;
                    SaveData.isParseData = true;
                    if (usefullBuffer[0] == 'A')
                    {
                        SaveData.isUsefull = true;
                    }
                    else if (usefullBuffer[0] == 'V')
                    {
                        SaveData.isUsefull = false;
                    }
                }
                else
                {
                    errorLog(GPS_SUBSTRINGNEXT_ERROR); //error
                }
            }
        }
    }
}

//Package gps content
void PackageGpsBuffer(void)
{
    if (SaveData.isParseData)
    {
        char TEMP_Gps_Buffer[Gps2Gsm_Buffer_Length] = {0};
        SaveData.isParseData = false;
        strcat(SaveData.Gps2Gsm_Buffer, "60A8768480014EBA53D151FA4E866C426551000D000A"); //您的老人发出了求救

        if (SaveData.isUsefull)
        {
            memset(TEMP_Gps_Buffer, 0, Gps2Gsm_Buffer_Length);
            Num_Ucs2Gbk_Exchange(SaveData.latitude);
            sprintf(TEMP_Gps_Buffer, "7EAC5EA6575068074E3AFF1A0020%s000D000A", SaveData.latitude); //经度坐标为：
            strcat(SaveData.Gps2Gsm_Buffer, TEMP_Gps_Buffer);

            memset(TEMP_Gps_Buffer, 0, Gps2Gsm_Buffer_Length);
            Num_Ucs2Gbk_Exchange(SaveData.longitude);
            sprintf(TEMP_Gps_Buffer, "7ECF5EA6575068074E3AFF1A0020%s000D000A", SaveData.longitude); //纬度坐标为：
            strcat(SaveData.Gps2Gsm_Buffer, TEMP_Gps_Buffer);
        }
        else
        {
            strcat(SaveData.Gps2Gsm_Buffer, "0047005000536570636E4E0D53EF7528000D000A"); //数据不可用
        }
    }
}
