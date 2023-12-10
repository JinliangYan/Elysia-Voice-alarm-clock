/**
* \file            clock.h
* \date            11/23/2023
* \brief           Header file for managing the clock and time-related functionalities
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_CLOCK_H
#define ELYSIA_VOICE_ALARM_CLOCK_CLOCK_H

#include "stm32f10x.h"
#include "../../config/clock_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* \brief           Enumeration for the time of day
*/
typedef enum clock_time_of_day {
   CLOCK_MORNING,      /*!< Morning time */
   CLOCK_AFTERNOON,    /*!< Afternoon time */
   CLOCK_EVENING,      /*!< Evening time */
   CLOCK_DUSK,         /*!< Dusk time */
   CLOCK_MIDNIGHT,     /*!< Midnight time */
} clock_time_of_day_t;

/**
* \brief           Enumeration for the season
*/
typedef enum clock_season {
   CLOCK_SPRING,       /*!< Spring season */
   CLOCK_SUMMER,       /*!< Summer season */
   CLOCK_AUTUMN,       /*!< Autumn season */
   CLOCK_WINTER,       /*!< Winter season */
} clock_season_t;

/**
* \brief           Year, month, day, hour, minute, second, and week information
*/
extern uint8_t clock_year, clock_month, clock_day, clock_hour, clock_minute, clock_second, clock_week;

/**
* \brief           Advice for the current time
*/
extern char* clock_advice;

/**
* \brief           Current time of day
*/
extern clock_time_of_day_t clock_time_of_day;

/**
* \brief           Current season
*/
extern clock_season_t clock_season;

/**
* \brief           Initializes the clock module
*/
void clock_init(void);

/**
* \brief           Updates the clock time
*/
void clock_update(void);

/**
* \brief           Checks if it's sleep time
* \return          1 if it's sleep time, 0 otherwise
*/
uint8_t clock_is_sleep_time(void);

/**
* \brief           Checks if it's time to get up
* \return          1 if it's time to get up, 0 otherwise
*/
uint8_t clock_is_getup_time(void);

/**
* \brief           Checks if it's your birthday
* \return          1 if it's your birthday, 0 otherwise
*/
uint8_t clock_is_my_birthday(void);

/**
* \brief           Checks if it's Elysia's birthday
* \return          1 if it's Elysia's birthday, 0 otherwise
*/
uint8_t clock_is_elysia_birthday(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_CLOCK_H */
