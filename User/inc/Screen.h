//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_HOMEPAGE_H
#define ElysiaVACLK_HOMEPAGE_H
#include "stm32f10x.h"

#define SCREEN_TYPE_NUM 2
typedef enum ScreenDef {
    SCREEN_TIME,
    SCREEN_MUSIC
} ScreenDef;

void Screen_Init(void);
void Screen_Update(void);
void Screen_Switch(ScreenDef newType);
ScreenDef Screen_GetType(void);

#endif //ElysiaVACLK_HOMEPAGE_H
