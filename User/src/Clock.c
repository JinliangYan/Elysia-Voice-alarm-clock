//
// Created by Jinliang on 11/23/2023.
//

#include "Clock.h"
#include "DS1302.h"

uint8_t Clock_Year, Clock_Month, Clock_Day, Clock_Hour, Clock_Minute, Clock_Second, Clock_Week;
char *Clock_Advice;

uint8_t Clock_SleepTime[2], Clock_GetUpTime[2];
uint8_t Clock_BirthDay_Me[2], Clock_BirthDay_Alysia[2];

Clock_TimeOfDayDef Clock_TimeOfDay;
Clock_SeasonDef Clock_Season;

static void Clock_DetermineTimeOfDay(void) {
    if (Clock_Hour >= 6 && Clock_Hour < 12) {
        Clock_TimeOfDay = CLOCK_MORNING;
    } else if (Clock_Hour >= 12 && Clock_Hour < 17) {
        Clock_TimeOfDay = CLOCK_AFTERNOON;
    } else if (Clock_Hour >= 17 && Clock_Hour < 19) {
        Clock_TimeOfDay = CLOCK_DUSK;
    } else if (Clock_Hour >= 19 && Clock_Hour < 22) {
        Clock_TimeOfDay = CLOCK_EVENING;
    } else if (Clock_Hour >= 22 || Clock_Hour < 6) {
        Clock_TimeOfDay = CLOCK_MIDNIGHT;
    }
}

static void Clock_DetermineSeason(void) {
    if (Clock_Month >= 3 && Clock_Month <= 5) {
        Clock_Season = CLOCK_SPRING;
    } else if (Clock_Month >= 6 && Clock_Month <= 8) {
        Clock_Season = CLOCK_SUMMER;
    } else if (Clock_Month >= 9 && Clock_Month <= 11) {
        Clock_Season = CLOCK_AUTUMN;
    } else {
        Clock_Season = CLOCK_WINTER;
    }
}

static void Clock_UserConfig(void) {
    /*配置睡觉时间*/
    Clock_SleepTime[0] = 11;
    Clock_SleepTime[1] = 10;
    /*配置起床时间*/
    Clock_GetUpTime[0] = 7;
    Clock_GetUpTime[1] = 30;
    /*设置生日*/
        /*自己生日*/
    Clock_BirthDay_Me[0] = 2;
    Clock_BirthDay_Me[1] = 6;
        /*爱莉生日*/
    Clock_BirthDay_Alysia[0] = 11;
    Clock_BirthDay_Alysia[1] = 11;
}

void Clock_Init(void) {
    Clock_UserConfig();
    DS1302_Init();
    Clock_Update();
}

void Clock_ReadDate(void) {
    DS1302_Read();
    Clock_Year = DS1302_Time[0];
    Clock_Month = DS1302_Time[1];
    Clock_Day = DS1302_Time[2];
    Clock_Hour = DS1302_Time[3];
    Clock_Minute = DS1302_Time[4];
    Clock_Second = DS1302_Time[5];
    Clock_Week = DS1302_Time[6];
}

void Clock_GetAdvice(void) {
    if (Clock_Hour >= 23) {
        Clock_Advice = "Sleep";
    } else if (Clock_Hour >= 22) {
        Clock_Advice = "Relax";
    } else if (Clock_Hour >= 19) {
        Clock_Advice = "Study";
    } else if (Clock_Hour >= 18) {
        Clock_Advice = "Eat! ";
    } else if (Clock_Hour >= 14) {
        Clock_Advice = "Study";
    } else if (Clock_Hour >= 13) {
        Clock_Advice = "Sleep";
    } else if (Clock_Hour >= 12) {
        Clock_Advice = "Eat! ";
    } else if (Clock_Hour >= 8) {
        Clock_Advice = "Study";
    } else if (Clock_Hour >= 7) {
        Clock_Advice = "Eat! ";
    } else if (Clock_Hour >= 0) {
        Clock_Advice = "Sleep";
    }
}

void Clock_Update(void) {
    Clock_ReadDate();
    Clock_GetAdvice();
    Clock_DetermineTimeOfDay();
    Clock_DetermineSeason();
}

uint8_t Clock_IsSleepTime(void) {
    return Clock_SleepTime[0] == Clock_Hour && Clock_SleepTime[1] == Clock_Minute;
}

uint8_t Clock_IsGetUpTime(void) {
    return Clock_GetUpTime[0] == Clock_Hour && Clock_GetUpTime[1] == Clock_Minute;
}

uint8_t Clock_IsMyBirthday(void) {
    return Clock_BirthDay_Me[0] == Clock_Month && Clock_BirthDay_Me[1] == Clock_Day;
}

uint8_t Clock_IsAlysiaBirthday(void) {
    return Clock_BirthDay_Alysia[0] == Clock_Month && Clock_BirthDay_Alysia[1] == Clock_Day;
}