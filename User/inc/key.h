/**
* \file            key.h
* \date            11/29/2023
* \brief           Header file for handling keypad input
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_KEY_H
#define ELYSIA_VOICE_ALARM_CLOCK_KEY_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* \brief           Enumeration for different keys on the keypad
*/
typedef enum Key_KeyDef {
   KEY_MODE = 0,               /*!< Mode key */
   KEY_PLAY_PAUSE,             /*!< Play/Pause key */
   KEY_VOICE_RESPONSE,         /*!< Voice response key */
   KEY_SET_TIME_ALARM,         /*!< Set time alarm key */
   KEY_VOLUME_PREV,            /*!< Volume previous key */
   KEY_VOLUME_NEXT,            /*!< Volume next key */
   KEY_TIME_DECREASE,          /*!< Time decrease key */
   KEY_TIME_INCREASE,          /*!< Time increase key */
} Key_KeyDef;

void key_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_KEY_H */
