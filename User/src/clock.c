/*
* \file            clock.c
* \date            11/23/2023
* \brief           Implementation of clock functionality and time-related operations.
*/

/*
* Copyright (c) 2023 JinLiang YAN
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* This file is part of Elysia-Voice-alarm-clock.
*
* Author:          JinLiang YAN <yanmiku0206@outlook.com>
*/

#include <stdio.h>
#include "clock.h"
#include "ds1302.h"

uint8_t clock_year, clock_month, clock_day, clock_hour, clock_minute, clock_second, clock_week;
char* clock_advice;

uint8_t clock_sleep_time[2], clock_get_up_time[2];
uint8_t clock_birthday_me[2], clock_birthday_elysia[2];

clock_time_of_day_t clock_time_of_day;
clock_season_t clock_season;

/**
* \brief           Determines the time of day based on the current hour
*/
static void
clock_determine_time_of_day(void) {
   if (clock_hour >= 6 && clock_hour < 12) {
       clock_time_of_day = CLOCK_MORNING;
   } else if (clock_hour >= 12 && clock_hour < 17) {
       clock_time_of_day = CLOCK_AFTERNOON;
   } else if (clock_hour >= 17 && clock_hour < 19) {
       clock_time_of_day = CLOCK_DUSK;
   } else if (clock_hour >= 19 && clock_hour < 22) {
       clock_time_of_day = CLOCK_EVENING;
   } else if (clock_hour >= 22 || clock_hour < 6) {
       clock_time_of_day = CLOCK_MIDNIGHT;
   }
}

/**
* \brief           Determines the season based on the current month
*/
static void
clock_determine_season(void) {
   if (clock_month >= 3 && clock_month <= 5) {
       clock_season = CLOCK_SPRING;
   } else if (clock_month >= 6 && clock_month <= 8) {
       clock_season = CLOCK_SUMMER;
   } else if (clock_month >= 9 && clock_month <= 11) {
       clock_season = CLOCK_AUTUMN;
   } else {
       clock_season = CLOCK_WINTER;
   }
}

/**
* \brief           Reads user-specific configuration settings
*/
static void
clock_user_config(void) {
   size_t t1, t2;

   sscanf(CLOCK_CFG_SLEEP_TIME, "%d:%d", &t1, &t2);
   clock_sleep_time[0] = t1;
   clock_sleep_time[1] = t2;

   sscanf(CLOCK_CFG_GETUP_TIME, "%d:%d", &t1, &t2);
   clock_get_up_time[0] = t1;
   clock_get_up_time[1] = t2;

   sscanf(CLOCK_CFG_BIRTHDAY, "%d-%d", &t1, &t2);
   clock_birthday_me[0] = t1;
   clock_birthday_me[1] = t2;

   clock_birthday_elysia[0] = 11;
   clock_birthday_elysia[1] = 11;
}

/**
* \brief           Initializes the clock module
*/
void
clock_init(void) {
   clock_user_config();
   ds1302_init();
   clock_update();
}

/**
* \brief           Reads the current date and time from the DS1302 RTC
*/
void
clock_read_date(void) {
   ds1302_read();
   clock_year = ds1302_time[0];
   clock_month = ds1302_time[1];
   clock_day = ds1302_time[2];
   clock_hour = ds1302_time[3];
   clock_minute = ds1302_time[4];
   clock_second = ds1302_time[5];
   clock_week = ds1302_time[6];
}

/**
* \brief           Generates advice based on the current time
*/
void
clock_get_advice(void) {
   if (clock_hour >= 23) {
       clock_advice = "Sleep";
   } else if (clock_hour >= 22) {
       clock_advice = "Relax";
   } else if (clock_hour >= 19) {
       clock_advice = "Study";
   } else if (clock_hour >= 18) {
       clock_advice = "Eat! ";
   } else if (clock_hour >= 14) {
       clock_advice = "Study";
   } else if (clock_hour >= 13) {
       clock_advice = "Sleep";
   } else if (clock_hour >= 12) {
       clock_advice = "Eat! ";
   } else if (clock_hour >= 8) {
       clock_advice = "Study";
   } else if (clock_hour >= 7) {
       clock_advice = "Eat! ";
   } else if (clock_hour >= 0) {
       clock_advice = "Sleep";
   }
}

/**
* \brief           Updates the clock information
*/
void
clock_update(void) {
   clock_read_date();
   clock_get_advice();
   clock_determine_time_of_day();
   clock_determine_season();
}

/**
* \brief           Checks if the current time is sleep time
* \return          Returns `1` if it's sleep time, `0` otherwise
*/
uint8_t
clock_is_sleep_time(void) {
   return clock_sleep_time[0] == clock_hour && clock_sleep_time[1] == clock_minute;
}

/**
* \brief           Checks if the current time is wake-up time
* \return          Returns `1` if it's wake-up time, `0` otherwise
*/
uint8_t
clock_is_getup_time(void) {
   return clock_get_up_time[0] == clock_hour && clock_get_up_time[1] == clock_minute;
}

/**
* \brief           Checks if it's the user's birthday
* \return          Returns `1` if it's the user's birthday, `0` otherwise
*/
uint8_t
clock_is_my_birthday(void) {
   return clock_birthday_me[0] == clock_month && clock_birthday_me[1] == clock_day;
}

/**
* \brief           Checks if it's Elysia's birthday
* \return          Returns `1` if it's Elysia's birthday, `0` otherwise
*/
uint8_t
clock_is_elysia_birthday(void) {
   return clock_birthday_elysia[0] == clock_month && clock_birthday_elysia[1] == clock_day;
}
