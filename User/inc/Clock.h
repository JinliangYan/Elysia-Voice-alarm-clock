//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_CLOCK_H
#define ElysiaVACLK_CLOCK_H

#include "stm32f10x.h"
#define CLOCK_Name "Elysia"

typedef enum Clock_TimeOfDayDef {
    CLOCK_MORNING,
    CLOCK_AFTERNOON,
    CLOCK_EVENING,
    CLOCK_DUSK,
    CLOCK_MIDNIGHT,
} Clock_TimeOfDayDef;

typedef enum Clock_SeasonDef {
    CLOCK_SPRING,
    CLOCK_SUMMER,
    CLOCK_AUTUMN,
    CLOCK_WINTER,
} Clock_SeasonDef;

extern uint8_t Clock_Year, Clock_Month, Clock_Day, Clock_Hour, Clock_Minute, Clock_Second, Clock_Week;
extern char *Clock_Advice;
extern Clock_TimeOfDayDef Clock_TimeOfDay;
extern Clock_SeasonDef Clock_Season;

void Clock_Init(void);
void Clock_Update(void);
uint8_t Clock_IsSleepTime(void);
uint8_t Clock_IsGetUpTime(void);
uint8_t Clock_IsMyBirthday(void);
uint8_t Clock_IsAlysiaBirthday(void);

#endif //ElysiaVACLK_CLOCK_H
