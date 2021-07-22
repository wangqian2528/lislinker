#ifndef GENERALPROTOCOL_HPP
#define GENERALPROTOCOL_HPP

#define NUMNER_LENGTH       8
#define MODE_LENGTH         1
#define MONTH_LENGTH        2
#define DAY_LENGTH          2
#define YEAR_LENGTH         4
#define HOUR_LENGTH         2
#define MINUTE_LENGTH       2
#define WBC_LENGTH          4
#define LYMPH_LENGTH        4
#define MON_LENGTH          4
#define GRAN_LENGTH         4
#define RBC_LENGTH          4
#define HGB_LENGTH          3
#define MCHC_LENGTH         4
#define MCV_LENGTH          4
#define MCH_LENGTH          4
#define RDW_LENGTH          3
#define HCT_LENGTH          3
#define PLT_LENGTH          4
#define MPV_LENGTH          3
#define PDW_LENGTH          3
#define PCT_LENGTH          3
#define RESERVEDA_LENGTH    4
#define EOS_LENGTH          3
#define RESERVEDB_LENGTH    7
#define TYPE_LENGTH         2

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    char number[8];
    char mode[1];
    char month[2];
    char day[2];
    char year[4];
    char hour[2];
    char minute[2];
    char wbc[4];
    char lymph[4];
    char mon[4];
    char gran[4];
    char lymph_[3];
    char mon_[3];
    char gran_[3];
    char rbc[4];
    char hgb[3];
    char mchc[4];
    char mcv[4];
    char mch[4];
    char rdw[3];
    char hct[3];
    char plt[4];
    char mpv[3];
    char pdw[3];
    char pct[3];
    char reserved1[4];
    char eos[3];
    char reserved2[7];
    char type[2];
    char reserved3[8];
    char regionL1[3];
    char regionL2[3];
    char regionL3[3];
    char regionL4[3];
    char regionL5[3];
    char regionL6[3];
    char regionL7[3];
    char regionL8[3];
    char reserved4[16];
    char wbcHisto[256];
    char rbcHisto[256];
    char pltHisto[256];
} SampleDataFrame;
#pragma pack(pop)

#endif // GENERALPROTOCOL_HPP
