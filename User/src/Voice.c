//
// Created by Jinliang on 11/23/2023.
//

#include "Voice.h"
#include "Clock.h"
#include "DFPLAYER_MINI.h"

#define USE_HARDWARE_DECODING
//#define USE_SOFTWARE_DECODING

/*定义语音类别和对应数目*/
    /*闲聊*/
#define VOICE_CHAT                                  8
#define VOICE_CHAT_NUM                              30
    /*天气*/
#define VOICE_WEATHER_RAIN                          1
#define VOICE_WEATHER_RAIN_NUM                      1

#define VOICE_WEATHER_SUNNY                         2
#define VOICE_WEATHER_SUNNY_NUM                     1

#define VOICE_WEATHER_COOL_DOWN                     3
#define VOICE_WEATHER_COOL_DOWN_NUM                 1
    /*情景*/

#define VOICE_BREAK_TIME                            4
#define VOICE_BREAK_TIME_NUM                        2

#define VOICE_SCENE_MISSION_ACCOMPLISHED            5
#define VOICE_SCENE_MISSION_ACCOMPLISHED_NUM        2

#define VOICE_SCENE_SLEEP_TIME                      6
#define VOICE_SCENE_SLEEP_TIME_NUM                  5

#define VOICE_SCENE_GETUP_TIME                      7
#define VOICE_SCENE_GETUP_TIME_NUM                  1

    /*时间*/
#define VOICE_TIME_DUSK                             9
#define VOICE_TIME_DUSK_NUM                         2

#define VOICE_TIME_WINTER                           10
#define VOICE_TIME_WINTER_NUM                       1

#define VOICE_TIME_MONDAY                           11
#define VOICE_TIME_WINTER_NUM                       1

#define VOICE_TIME_SUNDAY                           12
#define VOICE_TIME_SUNDAY_NUM                       1

#define VOICE_TIME_MORNING                          13
#define VOICE_TIME_MORNING_NUM                      4

#define VOICE_TIME_NIGHT                            14
#define VOICE_TIME_NIGHT_NUM                        2
    /*生日*/
#define VOICE_BIRTHDAY_ME                           15
#define VOICE_BIRTHDAY_ME_NUM                        2

#define VOICE_BIRTHDAY_ELYSIA                       16
#define VOICE_BIRTHDAY_ELYSIA_NUM                   2

#define VOICE_DEFAULT                               VOICE_CHAT
#define VOICE_DEFAULT_NUM                           VOICE_CHAT_NUM


static void Voice_Say(uint8_t category, uint8_t number) {
#ifdef USE_HARDWARE_DECODING
    DF_PlayFromFolder(category, number);
#endif
#ifdef USE_STDPERIPH_DRIVER

#endif
}

static uint8_t Voice_Random(uint8_t number) {
    return (Clock_Second + Clock_Minute + Clock_Hour) % number;
}

void Voice_Weather(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = 0;
//    if (天气判断) {
//        scene = ;
//        number = ;
//    }
//      ...
    Voice_Say(scene, number);
}

void Voice_Scene(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = 0;
    if (Clock_IsSleepTime()) {
        scene = VOICE_SCENE_SLEEP_TIME;
        number = Voice_Random(VOICE_SCENE_SLEEP_TIME_NUM);
    }
    else if (Clock_IsGetUpTime()) {
        scene = VOICE_SCENE_GETUP_TIME;
        number = Voice_Random(VOICE_SCENE_GETUP_TIME_NUM);
    }
//    else if (/*任务完成*/) {
//        scene = VOICE_SCENE_MISSION_ACCOMPLISHED;
//        number = Voice_Random(VOICE_SCENE_MISSION_ACCOMPLISHED_NUM);
//    }
    Voice_Say(scene, number);
}

void Voice_Chat(void) {
    uint8_t number = Voice_Random(VOICE_CHAT_NUM);
    Voice_Say(VOICE_CHAT, number);
}

void Voice_DayOfTime(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = 0;
    if (Clock_TimeOfDay == CLOCK_MORNING) {
        scene = VOICE_TIME_MORNING;
        number = Voice_Random(VOICE_TIME_MORNING_NUM);
    } else if (Clock_TimeOfDay == CLOCK_AFTERNOON) {
    } else if (Clock_TimeOfDay == CLOCK_DUSK) {
        scene = VOICE_TIME_DUSK;
        number = Voice_Random(VOICE_TIME_DUSK_NUM);
    } else if (Clock_TimeOfDay == CLOCK_EVENING) {
        scene = VOICE_TIME_NIGHT;
    } else if (Clock_TimeOfDay == CLOCK_NIGHT) {
        number = Voice_Random(VOICE_TIME_NIGHT_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_Season(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = 0;
    if (Clock_Season == CLOCK_WINTER) {
        scene = VOICE_TIME_WINTER;
        number = Voice_Random(VOICE_TIME_WINTER_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_BirthDay(uint8_t meOrAlysia) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = 0;
    if (meOrAlysia == 1) {
        scene = VOICE_BIRTHDAY_ME;
        number = Voice_Random(VOICE_BIRTHDAY_ME_NUM);
    } else {
        scene = VOICE_BIRTHDAY_ELYSIA;
        number = Voice_Random(VOICE_BIRTHDAY_ELYSIA_NUM);
    }
    Voice_Say(scene, number);
}

void Voice_Init(uint8_t volume) {
#ifdef USE_HARDWARE_DECODING
    DF_Init(volume);
#endif
#ifdef USE_STDPERIPH_DRIVER

#endif
}