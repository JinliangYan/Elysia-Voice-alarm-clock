//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_VOICE_H
#define ElysiaVACLK_VOICE_H

#include "stm32f10x.h"

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

#endif //ElysiaVACLK_VOICE_H
