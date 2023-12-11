/**
* \file            screen.h
* \date            11/23/2023
* \brief           Header file for managing the display screen
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_SCREEN_H
#define ELYSIA_VOICE_ALARM_CLOCK_SCREEN_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* \brief           Number of screen types
*/
#define SCREEN_TYPE_NUM     2

/**
* \brief           Enumeration for different screen types
*/
typedef enum screen {
   SCREEN_TIME,        /*!< Time screen */
   SCREEN_MUSIC        /*!< Music screen */
} screen_t;

/**
* \brief           Initializes the screen
*/
void screen_init(void);

/**
* \brief           Updates the screen
*/
void screen_update(void);

/**
* \brief           Switches the current screen to the specified type
* \param[in]       new_type: The new screen type
*/
void screen_switch(screen_t new_type);

/**
* \brief           Gets the current screen type
* \return          The current screen type
*/
screen_t screen_get_type(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_SCREEN_H */
