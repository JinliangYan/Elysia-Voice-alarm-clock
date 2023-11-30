//
// Created by Jinliang on 11/29/2023.
//

#ifndef ElysiaVAC_KEY_H
#define ElysiaVAC_KEY_H

#include "stm32f10x.h"

typedef enum Key_KeyDef {
    KEY_MODE = 0,
    KEY_POWER_PLAY,
    KEY_VOICE_RESPONSE,
    KEY_SET_TIME_ALARM,
    KEY_VOLUME_PREV,
    KEY_VOLUME_NEXT,
    KEY_TIME_DECREASE,
    KEY_TIME_INCREASE,
} Key_KeyDef;

void Key_Init(void);

#endif //ElysiaVAC_KEY_H
