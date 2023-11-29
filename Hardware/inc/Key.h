//
// Created by Jinliang on 11/27/2023.
//

#ifndef ElysiaVACLK_KEY_H
#define ElysiaVACLK_KEY_H

#include "stm32f10x.h"

void Key_Init(void);
uint8_t Key_Get(void);
void Key_Loop(void);

#endif //ElysiaVACLK_KEY_H
