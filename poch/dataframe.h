#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    char head[1];
    char code1[1];
    char code2[1];
    char devCode[1];
    char devID[40];
    char year[4];
    char month[2];
    char day[2];
    char info[1];
    char samID[15];
    char anaInfo[6];
    char species[1];
    char WBC[5];
    char RBC[5];
    char HGB[5];
    char HCT[5];
    char MCV[5];
    char MCH[5];
    char MCHC[5];
    char PLT[5];
    char LYM[5];
    char OTHR[5];
    char EO[5];
    char LYM_[5];
    char OTHR_[5];
    char EO_[5];
    char RDW_SD[5];
    char RDW_CV[5];
    char PDW[5];
    char MPV[5];
    char P_LCR[5];
    char tail[1];
}DataFrameD1;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    char head[1];
    char code1[1];
    char code2[1];
    char WBC_WL[100];
    char RBC_WL[100];
    char tail[1];
}DataFrameD2;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    char head[1];
    char code1[1];
    char code2[1];
    char PLT_WL[80];
    char WBC_LD[2];
    char WBC_T1[2];
    char WBC_T2[2];
    char WBC_UD[2];
    char RBC_LD[2];
    char RBC_UD[2];
    char PLT_LD[2];
    char PLT_UD[2];
    char RESERVE[128];
    char tail[1];
}DataFrameD3;
#pragma pack(pop)

#endif // DATAFRAME_H
