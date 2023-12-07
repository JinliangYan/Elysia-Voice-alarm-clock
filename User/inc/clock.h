//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_CLOCK_H
#define ElysiaVACLK_CLOCK_H

#include "stm32f10x.h"
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

#endif //ElysiaVACLK_CLOCK_H
