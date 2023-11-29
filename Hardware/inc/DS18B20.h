//
// Created by Jinliang on 11/23/2023.
//

#ifndef ElysiaVACLK_DS18B20_H
#define ElysiaVACLK_DS18B20_H

#include "stm32f10x.h"
void DS18B20_ConvertT();
float DS18B20_ReadT();
void DS18B20_Init(void);

#endif //ElysiaVACLK_DS18B20_H
