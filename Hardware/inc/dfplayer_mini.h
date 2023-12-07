/*
 * DFPLAYER_MINI.h
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#ifndef INC_DFPLAYER_MINI_H_
#define INC_DFPLAYER_MINI_H_

#include "stm32f10x.h"

void df_init(uint8_t volume);
void df_pause(void);
void df_continue(void);
void df_play_from_folder(uint8_t folder, uint8_t number);
void df_loop_from_folder(uint8_t folder);
void df_set_volume(uint8_t volume);

#endif /* INC_DFPLAYER_MINI_H_ */
