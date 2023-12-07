//
// Created by Jinliang on 11/23/2023.
//

#include "clock.h"
#include "ds1302.h"

uint8_t clock_year, clock_month, clock_day, clock_hour, clock_minute, clock_second, clock_week;
char* clock_advice;

uint8_t clock_sleep_time[2], clock_get_up_time[2];
uint8_t clock_birthday_me[2], clock_birthday_elysia[2];

clock_time_of_day_t clock_time_of_day;
clock_season_t clock_season;

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

static void
clock_user_config(void) {
    /*配置睡觉时间*/
    clock_sleep_time[0] = 11;
    clock_sleep_time[1] = 10;
    /*配置起床时间*/
    clock_get_up_time[0] = 7;
    clock_get_up_time[1] = 30;
    /*设置生日*/
    /*自己生日*/
    clock_birthday_me[0] = 2;
    clock_birthday_me[1] = 6;
    /*爱莉生日*/
    clock_birthday_elysia[0] = 11;
    clock_birthday_elysia[1] = 11;
}

void
clock_init(void) {
    clock_user_config();
    ds1302_init();
    clock_update();
}

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

void
clock_update(void) {
    clock_read_date();
    clock_get_advice();
    clock_determine_time_of_day();
    clock_determine_season();
}

uint8_t
clock_is_sleep_time(void) {
    return clock_sleep_time[0] == clock_hour && clock_sleep_time[1] == clock_minute;
}

uint8_t
clock_is_getup_time(void) {
    return clock_get_up_time[0] == clock_hour && clock_get_up_time[1] == clock_minute;
}

uint8_t
clock_is_my_birthday(void) {
    return clock_birthday_me[0] == clock_month && clock_birthday_me[1] == clock_day;
}

uint8_t
clock_is_elysia_birthday(void) {
    return clock_birthday_elysia[0] == clock_month && clock_birthday_elysia[1] == clock_day;
}