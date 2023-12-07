/**
* \file            voice.h
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

#ifndef ElysiaVACLK_VOICE_H
#define ElysiaVACLK_VOICE_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum voice_status {
    VOICE_ON,
    VOICE_OFF,
} voice_status_t;

void voice_invoke(void);
void voice_chat(void);
void voice_day_of_time(void);
void voice_scene(void);
void voice_weather(void);
void voice_season(void);
void voice_init(uint8_t volume);
void voice_birthday(uint8_t meOrAlysia);
void voice_set_volume(uint16_t volume);
void voice_volume_increase(void);
void voice_volume_decrease(void);
void voice_on(void);
void voice_off(void);

void voice_music_play(void);
void voice_music_pause(void);
void voice_music_continue(void);
void voice_music_next(void);
void voice_music_previous(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ElysiaVACLK_VOICE_H */
