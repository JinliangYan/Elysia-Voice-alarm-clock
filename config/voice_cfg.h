/**
* \file            voice_cfg.h
* \date            12/10/2023
* \brief           Audio configuration file
*
* This file contains the category folder number of the audio file,
* and the number of audio files under the corresponding category.
* It is configured by the user (if needed).
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_VOICE_CFG_H
#define ELYSIA_VOICE_ALARM_CLOCK_VOICE_CFG_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Define voice categories and corresponding numbers */
/* Weather */
#define VOICE_WEATHER_BASE                0
#define VOICE_WEATHER_RAIN                (VOICE_WEATHER_BASE + 1)
#define VOICE_WEATHER_RAIN_NUM            2 // Number of rainy weather responses
#define VOICE_WEATHER_SUNNY               (VOICE_WEATHER_BASE + 2)
#define VOICE_WEATHER_SUNNY_NUM           2 // Number of sunny weather responses
#define VOICE_WEATHER_COOL_DOWN           (VOICE_WEATHER_BASE + 3)
#define VOICE_WEATHER_COOL_DOWN_NUM       1 // Number of cool-down weather responses

/* Scene */
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

/* Interaction */
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

/* Time */
#define VOICE_TIME_BASE                   30
#define VOICE_TIME_MORNING_GREETING       (VOICE_TIME_BASE + 1)
#define VOICE_TIME_MORNING_GREETING_NUM   7 // Number of morning greeting responses
#define VOICE_TIME_EVENING                (VOICE_TIME_BASE + 2)
#define VOICE_TIME_EVENING_NUM            5 // Number of evening responses
#define VOICE_TIME_MIDNIGHT               (VOICE_TIME_BASE + 3)
#define VOICE_TIME_MIDNIGHT_NUM           2 // Number of midnight responses

/* Season */
#define VOICE_SEASON_BASE                 40
#define VOICE_SEASON_WINTER               (VOICE_SEASON_BASE + 1)
#define VOICE_SEASON_WINTER_NUM           2 // Number of responses for winter season
#define VOICE_SEASON_AUTUMN               (VOICE_SEASON_BASE + 2)
#define VOICE_SEASON_AUTUMN_NUM           1 // Number of responses for autumn season
#define VOICE_SEASON_SUMMER               (VOICE_SEASON_BASE + 3)
#define VOICE_SEASON_SUMMER_NUM           1 // Number of responses for summer season

/* Miscellaneous */
#define VOICE_MISC_BASE                   80
#define VOICE_MISC_CHARACTER_BIRTHDAY     (VOICE_MISC_BASE + 1)
#define VOICE_MISC_CHARACTER_BIRTHDAY_NUM 2 // Number of responses for character birthday
#define VOICE_MISC_BIRTHDAY               (VOICE_MISC_BASE + 2)
#define VOICE_MISC_BIRTHDAY_NUM           2 // Number of responses for general birthday
#define VOICE_MISC_MONDAY                 (VOICE_MISC_BASE + 3)
#define VOICE_MISC_MONDAY_NUM             1 // Number of responses for Monday

/* Default */
#define VOICE_DEFAULT                     VOICE_INTERACTION_CHAT
#define VOICE_DEFAULT_NUM                 VOICE_INTERACTION_CHAT_NUM

/* Music */
#define VOICE_MUSIC_RESOURCE              80
#define VOICE_MUSIC_NUM                   84

#define VOICE_VOLUME_MAX                  (30)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_VOICE_CFG_H */
