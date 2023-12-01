//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_VOICE_H
#define ElysiaVACLK_VOICE_H

#include "stm32f10x.h"

typedef enum Voice_StatusDef {
    VOICE_ON,
    VOICE_OFF,
} Voice_StatusDef;

void Voice_Invoke(void);
void Voice_Chat(void);
void Voice_DayOfTime(void);
void Voice_Scene(void);
void Voice_Weather(void);
void Voice_Season(void);
void Voice_Init(uint8_t volume);
void Voice_BirthDay(uint8_t meOrAlysia);
void Voice_SetVolume(uint16_t volume);
void Voice_VolumeIncrease(void);
void Voice_VolumeDecrease(void);
void Voice_ON(void);
void Voice_OFF(void);

void Voice_MusicPlay(void);
void Voice_MusicPause(void);
void Voice_MusicContinue(void);
void Voice_MusicNext(void);
void Voice_MusicPrevious(void);

#endif //ElysiaVACLK_VOICE_H
