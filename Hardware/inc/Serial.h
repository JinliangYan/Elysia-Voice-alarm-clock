//
// Created by Jinliang on 11/21/2023.
//

#ifndef CLIONSTM32DEMO_SERIAL_H
#define CLIONSTM32DEMO_SERIAL_H

#include "stm32f10x.h"
#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
#endif //CLIONSTM32DEMO_SERIAL_H
