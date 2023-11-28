//
// Created by Jinliang on 11/23/2023.
//

#ifndef CLIONSTM32DEMO_VOICE_H
#define CLIONSTM32DEMO_VOICE_H

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
void Voice_ON(void);
void Voice_OFF(void);

#endif //CLIONSTM32DEMO_VOICE_H
