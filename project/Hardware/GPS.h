#ifndef _GPS_H_
#define _GPS_H_
#include <string.h>
#include <stdio.h>
#include "USART.h"
#include "mylib.h"
#include "LED.h"

#define GPS_Buffer_Length 200
#define UTCTime_Length 11
#define latitude_Length 50
#define N_S_Length 2
#define longitude_Length 50
#define E_W_Length 2
#define Gps2Gsm_Buffer_Length 300

typedef struct
{
    char GPS_Buffer[GPS_Buffer_Length];
    char isGetData; //
    char isParseData;
    char UTCTime[UTCTime_Length];
    char latitude[latitude_Length];
    char N_S[N_S_Length];
    char longitude[longitude_Length];
    char E_W[E_W_Length];
    char isUsefull;
    char Gps2Gsm_Buffer[Gps2Gsm_Buffer_Length];
} SaveData_Typedef;

extern SaveData_Typedef SaveData;
extern u16 Usart3_Point;

void Clr_Struct(void);
void ParseGpsBuffer(void);
void PackageGpsBuffer(void);

#endif
