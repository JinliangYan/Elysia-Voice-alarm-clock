/**
* \file            clock.h
* \date            11/23/2023
* \brief
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

#ifndef ElysiaVACLK_CLOCK_H
#define ElysiaVACLK_CLOCK_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CLOCK_NAME "Elysia"

typedef enum clock_time_of_day {
    CLOCK_MORNING,
    CLOCK_AFTERNOON,
    CLOCK_EVENING,
    CLOCK_DUSK,
    CLOCK_MIDNIGHT,
} clock_time_of_day_t;

typedef enum clock_season {
    CLOCK_SPRING,
    CLOCK_SUMMER,
    CLOCK_AUTUMN,
    CLOCK_WINTER,
} clock_season_t;

extern uint8_t clock_year, clock_month, clock_day, clock_hour, clock_minute, clock_second, clock_week;
extern char* clock_advice;
extern clock_time_of_day_t clock_time_of_day;
extern clock_season_t clock_season;

void clock_init(void);
void clock_update(void);
uint8_t clock_is_sleep_time(void);
uint8_t clock_is_getup_time(void);
uint8_t clock_is_my_birthday(void);
uint8_t clock_is_elysia_birthday(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //ElysiaVACLK_CLOCK_H
