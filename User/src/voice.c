/**
 * \file            voice.c
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

#include "voice.h"
#include "clock.h"
#include "dfplayer_mini.h"

#define LOG_TAG "VOICE"
#include "elog.h"

/* 定义语音类别和对应数目 */
/* 天气 */
#define VOICE_WEATHER_BASE                0
#define VOICE_WEATHER_RAIN                (VOICE_WEATHER_BASE + 1)
#define VOICE_WEATHER_RAIN_NUM            2 // Number of rainy weather responses
#define VOICE_WEATHER_SUNNY               (VOICE_WEATHER_BASE + 2)
#define VOICE_WEATHER_SUNNY_NUM           2 // Number of sunny weather responses
#define VOICE_WEATHER_COOL_DOWN           (VOICE_WEATHER_BASE + 3)
#define VOICE_WEATHER_COOL_DOWN_NUM       1 // Number of cool-down weather responses

/* 情景 */
#define VOICE_SCENE_BASE                  10
#define VOICE_SCENE_REST_TIME             (VOICE_SCENE_BASE + 1)
#define VOICE_SCENE_REST_TIME_NUM         7 // Number of responses for rest time scene
#define VOICE_SCENE_TASK_SET              (VOICE_SCENE_BASE + 2)
#define VOICE_SCENE_TASK_SET_NUM          1 // Number of responses for task set scene
#define VOICE_SCENE_TASK_ACCOMPLISHED     (VOICE_SCENE_BASE + 3)
#define VOICE_SCENE_TASK_ACCOMPLISHED_NUM 4 // Number of responses for task accomplished scene
#define VOICE_SCENE_GREETING              (VOICE_SCENE_BASE + 4)
#define VOICE_SCENE_GREETING_NUM          9 // Number of responses for greeting scene
#define VOICE_SCENE_WAKE_UP               (VOICE_SCENE_BASE + 5)
#define VOICE_SCENE_WAKE_UP_NUM           2 // Number of responses for wake-up scene
#define VOICE_SCENE_HANG_OUT              (VOICE_SCENE_BASE + 6)
#define VOICE_SCENE_HANG_OUT_NUM          5 // Number of responses for hang-out scene
#define VOICE_SCENE_FAILURE               (VOICE_SCENE_BASE + 7)
#define VOICE_SCENE_FAILURE_NUM           1 // Number of responses for failure scene

/* 互动 */
#define VOICE_INTERACTION_BASE            20
#define VOICE_INTERACTION_CHAT            (VOICE_INTERACTION_BASE + 1)
#define VOICE_INTERACTION_CHAT_NUM        84 // Number of chat responses
#define VOICE_INTERACTION_EAT             (VOICE_INTERACTION_BASE + 2)
#define VOICE_INTERACTION_EAT_NUM         5 // Number of responses for eating interaction
#define VOICE_INTERACTION_LIFT            (VOICE_INTERACTION_BASE + 3)
#define VOICE_INTERACTION_LIFT_NUM        2 // Number of responses for lifting interaction
#define VOICE_INTERACTION_NEW_CLOTHES     (VOICE_INTERACTION_BASE + 4)
#define VOICE_INTERACTION_NEW_CLOTHES_NUM 7 // Number of responses for new clothes interaction
#define VOICE_INTERACTION_SHAKE           (VOICE_INTERACTION_BASE + 5)
#define VOICE_INTERACTION_SHAKE_NUM       8 // Number of responses for shaking interaction
#define VOICE_INTERACTION_THANKS          (VOICE_INTERACTION_BASE + 6)
#define VOICE_INTERACTION_THANKS_NUM      3 // Number of responses for thanks interaction

/* 时间 */
#define VOICE_TIME_BASE                   30
#define VOICE_TIME_MORNING_GREETING       (VOICE_TIME_BASE + 1)
#define VOICE_TIME_MORNING_GREETING_NUM   7 // Number of morning greeting responses
#define VOICE_TIME_EVENING                (VOICE_TIME_BASE + 2)
#define VOICE_TIME_EVENING_NUM            5 // Number of evening responses
#define VOICE_TIME_MIDNIGHT               (VOICE_TIME_BASE + 3)
#define VOICE_TIME_MIDNIGHT_NUM           2 // Number of midnight responses

/* 季节 */
#define VOICE_SEASON_BASE                 40
#define VOICE_SEASON_WINTER               (VOICE_SEASON_BASE + 1)
#define VOICE_SEASON_WINTER_NUM           2 // Number of responses for winter season
#define VOICE_SEASON_AUTUMN               (VOICE_SEASON_BASE + 2)
#define VOICE_SEASON_AUTUMN_NUM           1 // Number of responses for autumn season
#define VOICE_SEASON_SUMMER               (VOICE_SEASON_BASE + 3)
#define VOICE_SEASON_SUMMER_NUM           1 // Number of responses for summer season

/* 杂项 */
#define VOICE_MISC_BASE                   80
#define VOICE_MISC_CHARACTER_BIRTHDAY     (VOICE_MISC_BASE + 1)
#define VOICE_MISC_CHARACTER_BIRTHDAY_NUM 2 // Number of responses for character birthday
#define VOICE_MISC_BIRTHDAY               (VOICE_MISC_BASE + 2)
#define VOICE_MISC_BIRTHDAY_NUM           2 // Number of responses for general birthday
#define VOICE_MISC_MONDAY                 (VOICE_MISC_BASE + 3)
#define VOICE_MISC_MONDAY_NUM             1 // Number of responses for Monday


/* 默认 */
#define VOICE_DEFAULT                     VOICE_INTERACTION_CHAT
#define VOICE_DEFAULT_NUM                 VOICE_INTERACTION_CHAT_NUM

/*音乐*/
#define VOICE_MUSIC_RESOURCE              80
#define VOICE_MUSIC_NUM                   84

#define VOICE_VOLUME_MAX                  (30)

static voice_status_t voice_status;
static uint8_t voice_volume;

static int8_t voice_music_now = 1;

static void
voice_say(uint8_t category, uint8_t number) {
    if (voice_status == VOICE_OFF) {
        return;
    }
    df_play_from_folder(category, number);
    log_i("df_play_from_folder(%d, %d) Invoked", category, number);
}

static uint8_t
voice_random(uint8_t number) {
    return (clock_second + clock_minute + clock_hour) % number + 1;
}

void
voice_weather(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    ;
    //    Replace天气判断 with your actual weather condition check
    //    if (/*天气判断*/) {
    //        scene = VOICE_WEATHER_RAIN; // Or VOICE_WEATHER_SUNNY, etc.
    //        number = voice_random(VOICE_WEATHER_NUM);
    //    }
    voice_say(scene, number);
}

void
voice_scene(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    ;
    if (clock_is_sleep_time()) {
        scene = VOICE_TIME_MIDNIGHT;
        number = voice_random(VOICE_TIME_MIDNIGHT_NUM);
    } else if (clock_is_getup_time()) {
        scene = VOICE_SCENE_WAKE_UP;
        number = voice_random(VOICE_SCENE_WAKE_UP);
    }
    // else if (/*任务完成*/) {
    //     scene = VOICE_SCENE_MISSION_ACCOMPLISHED;
    //     number = voice_random(VOICE_SCENE_NUM);
    // }
    voice_say(scene, number);
}

void
voice_chat(void) {
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    voice_say(VOICE_INTERACTION_CHAT, number);
}

void
voice_day_of_time(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    ;
    if (clock_time_of_day == CLOCK_MORNING) {
        scene = VOICE_TIME_MORNING_GREETING;
        number = voice_random(VOICE_TIME_MORNING_GREETING_NUM);
    } else if (clock_time_of_day == CLOCK_AFTERNOON) {
        // Handle afternoon
    } else if (clock_time_of_day == CLOCK_DUSK) {
        // Handle dusk
    } else if (clock_time_of_day == CLOCK_EVENING) {
        scene = VOICE_TIME_EVENING;
        number = voice_random(VOICE_TIME_EVENING_NUM);
    } else if (clock_time_of_day == CLOCK_MIDNIGHT) {
        scene = VOICE_TIME_MIDNIGHT;
        number = voice_random(VOICE_TIME_MIDNIGHT_NUM);
    }
    voice_say(scene, number);
}

void
voice_season(void) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    ;
    if (clock_season == CLOCK_WINTER) {
        scene = VOICE_SEASON_WINTER;
        number = voice_random(VOICE_SEASON_WINTER_NUM);
    }
    voice_say(scene, number);
}

void
voice_birthday(uint8_t meOrAlysia) {
    uint8_t scene = VOICE_DEFAULT;
    uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
    ;
    if (meOrAlysia == 0 && clock_is_elysia_birthday()) {
        scene = VOICE_MISC_CHARACTER_BIRTHDAY;
        number = voice_random(VOICE_MISC_CHARACTER_BIRTHDAY_NUM);
    } else if (meOrAlysia == 1 && clock_is_my_birthday()) {
        scene = VOICE_MISC_BIRTHDAY;
        number = voice_random(VOICE_MISC_BIRTHDAY_NUM);
    }
    voice_say(scene, number);
}

void
voice_invoke(void) {
    // 随机选择要触发的函数
    int randomFunction = voice_random(5); // 选择 0 到 5 之间的随机数

    // 根据随机数调用相应的函数
    switch (randomFunction) {
        case 0: voice_weather(); break;
        case 1: voice_scene(); break;
        case 2: voice_chat(); break;
        case 3: voice_day_of_time(); break;
        case 4: voice_season(); break;
        case 5:
            voice_birthday(0); // 传递 0 表示触发角色生日
            voice_birthday(1);
            break;
            // 添加其他函数触发的 case，如果有的话
        default: voice_chat();
    }
}

void
voice_on(void) {
    voice_status = VOICE_ON;
}

void
voice_off(void) {
    voice_status = VOICE_OFF;
}

void
voice_set_volume(uint16_t volume) {
    voice_volume = volume;
    df_set_volume(volume);
}

void
voice_volume_increase(void) {
    if (voice_volume == VOICE_VOLUME_MAX) {
        return;
    }
    voice_volume++;
    df_set_volume(voice_volume);
}

void
voice_volume_decrease(void) {
    if (voice_volume == 0) {
        return;
    }
    voice_volume--;
    df_set_volume(voice_volume);
}

///////////////////////////////////////////////////////////////
////////////////////////// MUSIC //////////////////////////////
void
voice_music_play(void) {
    log_i("voice_music_play invoked");
    df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}

void
voice_music_pause(void) {
    log_i("voice_music_pause invoked");
    df_pause();
}

void
voice_music_continue(void) {
    log_i("voice_music_continue invoked");
    df_continue();
}

void
voice_music_next(void) {
    log_i("voice_music_next invoked");
    voice_music_now++;
    if (voice_music_now > VOICE_MUSIC_NUM) {
        voice_music_now = 1;
    }
    df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}

void
voice_music_previous(void) {
    log_i("voice_music_previous invoked");
    voice_music_now--;
    if (voice_music_now < 1) {
        voice_music_now = VOICE_MUSIC_NUM;
    }
    df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}

///////////////////////////////////////////////////////////////

void
voice_init(uint8_t volume) {
    voice_volume = volume;
    df_init(volume);
    voice_status = VOICE_ON;
}