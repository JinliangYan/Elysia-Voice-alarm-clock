/*
 * DFPLAYER_MINI.h
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#ifndef INC_DFPLAYER_MINI_H_
#define INC_DFPLAYER_MINI_H_

#include "stm32f10x.h"

void DF_Init (uint8_t volume);
void DF_Pause (void);
void DF_Continue (void);
void DF_PlayFromFolder(uint8_t folder, uint8_t number);
void DF_LoopFromFolder(uint8_t folder);
void DF_SetVolume(uint8_t volume);

#endif /* INC_DFPLAYER_MINI_H_ */
