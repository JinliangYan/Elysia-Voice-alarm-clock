//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_HOMEPAGE_H
#define ElysiaVACLK_HOMEPAGE_H
#include "stm32f10x.h"

#define SCREEN_TYPE_NUM 2

typedef enum screen { SCREEN_TIME, SCREEN_MUSIC } screen_t;

void screen_init(void);
void screen_update(void);
void screen_switch(screen_t newType);
screen_t screen_get_type(void);

#endif //ElysiaVACLK_HOMEPAGE_H
