/**
* \file            voice.h
* \date            11/23/2023
* \brief           Header file for managing voice interactions and music playback
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_VOICE_H
#define ELYSIA_VOICE_ALARM_CLOCK_VOICE_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
* \brief           Enumeration for voice status
*/
typedef enum voice_status {
   VOICE_ON,       /*!< Voice is on */
   VOICE_OFF,      /*!< Voice is off */
} voice_status_t;

/**
* \brief           Invokes a random voice interaction
*/
void voice_invoke(void);

/**
* \brief           Initiates a chat voice interaction
*/
void voice_chat(void);

/**
* \brief           Initiates a voice interaction based on the time of the day
*/
void voice_day_of_time(void);

/**
* \brief           Initiates a voice interaction based on the current scene
*/
void voice_scene(void);

/**
* \brief           Initiates a voice interaction based on the weather
*/
void voice_weather(void);

/**
* \brief           Initiates a voice interaction based on the season
*/
void voice_season(void);

/**
* \brief           Initializes the voice module with the specified volume
* \param[in]       volume: The initial volume level
*/
void voice_init(uint8_t volume);

/**
* \brief           Initiates a voice interaction related to birthdays
* \param[in]       meOrAlysia: 0 for character birthday, 1 for general birthday
*/
void voice_birthday(uint8_t meOrAlysia);

/**
* \brief           Sets the volume level for voice interactions
* \param[in]       volume: The desired volume level
*/
void voice_set_volume(uint16_t volume);

/**
* \brief           Increases the volume level for voice interactions
*/
void voice_volume_increase(void);

/**
* \brief           Decreases the volume level for voice interactions
*/
void voice_volume_decrease(void);

/**
* \brief           Turns on the voice module
*/
void voice_on(void);

/**
* \brief           Turns off the voice module
*/
void voice_off(void);

/**
* \brief           Initiates playing the next music track
*/
void voice_music_play(void);

/**
* \brief           Pauses the currently playing music track
*/
void voice_music_pause(void);

/**
* \brief           Continues playing the paused music track
*/
void voice_music_continue(void);

/**
* \brief           Plays the next music track in the playlist
*/
void voice_music_next(void);

/**
* \brief           Plays the previous music track in the playlist
*/
void voice_music_previous(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_VOICE_H */
