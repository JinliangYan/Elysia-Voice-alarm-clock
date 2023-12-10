/**
 * \file            voice.c
 * \date            11/23/2023
 * \brief           Implementation of the voice module.
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
#include "../../config/voice_cfg.h"
#include "clock.h"
#include "dfplayer_mini.h"

#define LOG_TAG "VOICE"
#include "elog.h"

/* Static variables */
static voice_status_t voice_status;       /*!< Current voice status */
static uint8_t voice_volume;               /*!< Current voice volume */

static int8_t voice_music_now = 1;        /*!< Current playing music index */

/**
* \brief           Say a phrase from the specified voice category and number.
* \param[in]       category: Voice category
* \param[in]       number: Voice number within the category
*/
static void
voice_say(uint8_t category, uint8_t number) {
   if (voice_status == VOICE_OFF) {
       return;
   }
   df_play_from_folder(category, number);
   log_i("df_play_from_folder(%d, %d) Invoked", category, number);
}

/**
* \brief           Generate a random number within the given range.
* \param[in]       number: Maximum value for the random number
* \return          Random number within the range [1, number]
*/
static uint8_t
voice_random(uint8_t number) {
   return (clock_second + clock_minute + clock_hour) % number + 1;
}

/**
* \brief           Speak about the current weather condition.
*/
void
voice_weather(void) {
   uint8_t scene = VOICE_DEFAULT;
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
   // Replace 天气判断 with your actual weather condition check
   // if (/*天气判断*/) {
   //     scene = VOICE_WEATHER_RAIN; // Or VOICE_WEATHER_SUNNY, etc.
   //     number = voice_random(VOICE_WEATHER_NUM);
   // }
   voice_say(scene, number);
}

/**
* \brief           Speak about the current scene.
*/
void
voice_scene(void) {
   uint8_t scene = VOICE_DEFAULT;
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
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

/**
* \brief           Initiate a chat.
*/
void
voice_chat(void) {
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
   voice_say(VOICE_INTERACTION_CHAT, number);
}

/**
* \brief           Speak about the current time of day.
*/
void
voice_day_of_time(void) {
   uint8_t scene = VOICE_DEFAULT;
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
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

/**
* \brief           Speak about the current season.
*/
void
voice_season(void) {
   uint8_t scene = VOICE_DEFAULT;
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
   if (clock_season == CLOCK_WINTER) {
       scene = VOICE_SEASON_WINTER;
       number = voice_random(VOICE_SEASON_WINTER_NUM);
   }
   voice_say(scene, number);
}

/**
* \brief           Speak about a birthday event.
* \param[in]       meOrAlysia: Specify whose birthday event to speak about (0 for Elysia, 1 for the user)
*/
void
voice_birthday(uint8_t meOrAlysia) {
   uint8_t scene = VOICE_DEFAULT;
   uint8_t number = voice_random(VOICE_INTERACTION_CHAT_NUM);
   if (meOrAlysia == 0 && clock_is_elysia_birthday()) {
       scene = VOICE_MISC_CHARACTER_BIRTHDAY;
       number = voice_random(VOICE_MISC_CHARACTER_BIRTHDAY_NUM);
   } else if (meOrAlysia == 1 && clock_is_my_birthday()) {
       scene = VOICE_MISC_BIRTHDAY;
       number = voice_random(VOICE_MISC_BIRTHDAY_NUM);
   }
   voice_say(scene, number);
}

/**
* \brief           Randomly invoke one of the voice-related functions.
*/
void
voice_invoke(void) {
   /* Randomly select the function to trigger */
   int randomFunction = voice_random(5); // Select a random number between 0 and 5

   /* Call the corresponding function based on the random number */
   switch (randomFunction) {
       case 0: voice_weather(); break;
       case 1: voice_scene(); break;
       case 2: voice_chat(); break;
       case 3: voice_day_of_time(); break;
       case 4: voice_season(); break;
       case 5:
           voice_birthday(0); /* Pass 0 to trigger character's birthday */
           voice_birthday(1);
           break;
           /* Add cases for other functions to trigger, if any */
       default: voice_chat();
   }
}

/**
* \brief           Turn on the voice module.
*/
void
voice_on(void) {
   voice_status = VOICE_ON;
}

/**
* \brief           Turn off the voice module.
*/
void
voice_off(void) {
   voice_status = VOICE_OFF;
}

/**
* \brief           Set the volume of the voice module.
* \param[in]       volume: New volume level
*/
void
voice_set_volume(uint16_t volume) {
   voice_volume = volume;
   df_set_volume(volume);
}

/**
* \brief           Increase the volume of the voice module.
*/
void
voice_volume_increase(void) {
   if (voice_volume == VOICE_VOLUME_MAX) {
       return;
   }
   voice_volume++;
   df_set_volume(voice_volume);
}

/**
* \brief           Decrease the volume of the voice module.
*/
void
voice_volume_decrease(void) {
   if (voice_volume == 0) {
       return;
   }
   voice_volume--;
   df_set_volume(voice_volume);
}

/* Music handler functions */
/**
* \brief           Play the currently selected music.
*/
void
voice_music_play(void) {
   log_i("voice_music_play invoked");
   df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}

/**
* \brief           Pause the currently playing music.
*/
void
voice_music_pause(void) {
   log_i("voice_music_pause invoked");
   df_pause();
}

/**
* \brief           Continue playing the paused music.
*/
void
voice_music_continue(void) {
   log_i("voice_music_continue invoked");
   df_continue();
}

/**
* \brief           Play the next music track.
*/
void
voice_music_next(void) {
   log_i("voice_music_next invoked");
   voice_music_now++;
   if (voice_music_now > VOICE_MUSIC_NUM) {
       voice_music_now = 1;
   }
   df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}

/**
* \brief           Play the previous music track.
*/
void
voice_music_previous(void) {
   log_i("voice_music_previous invoked");
   voice_music_now--;
   if (voice_music_now < 1) {
       voice_music_now = VOICE_MUSIC_NUM;
   }
   df_play_from_folder(VOICE_MUSIC_RESOURCE, voice_music_now);
}
/* Music handler functions end */

/**
* \brief           Initialize the voice module with the specified volume.
* \param[in]       volume: Initial volume level
*/
void
voice_init(uint8_t volume) {
   voice_volume = volume;
   df_init(volume);
   voice_status = VOICE_ON;
}
