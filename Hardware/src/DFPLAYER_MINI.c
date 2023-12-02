/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#include "stm32f10x.h"
#include "Serial.h"

#define LOG_TAG "DFPLAYER_MINI"
#include "elog.h"

#define DF_UART USART1

#define Source      0x02  // TF CARD

#define Previous_Key   GPIO_PIN_1
#define Previous_Port  GPIOA
#define Pause_Key      GPIO_PIN_2
#define Pause_Port     GPIOA
#define Next_Key       GPIO_PIN_3
#define Next_Port      GPIOA

/*************************************** NO CHANGES AFTER THIS *************************************************/

int ispause =0;
int isplaying=1;


# define Start_Byte 0x7E
# define End_Byte   0xEF
# define Version    0xFF
# define Cmd_Len    0x06
# define Feedback   0x00    //If need for Feedback: 0x01,  No Feedback: 0

void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
    log_d("Send_cmd{cmd = 0x%02X, p1 = %d, p2 = %d}", cmd, Parameter1, Parameter2);
	uint16_t Checksum = Version + Cmd_Len + cmd + Feedback + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

	uint8_t CmdSequence[10] = { Start_Byte, Version, Cmd_Len, cmd, Feedback, Parameter1, Parameter2, (Checksum>>8)&0x00ff, (Checksum&0x00ff), End_Byte};

    Serial_SendArray(CmdSequence, 10);
}

void DF_PlayFromStart(void)
{
    Send_cmd(0x03,0x00,0x01);
}


void DF_Init (uint8_t volume) // 0~30
{
    Serial_Init();
	Send_cmd(0x3F, 0x00, Source);
	Send_cmd(0x06, 0x00, volume);
}

void DF_SetVolume(uint8_t volume) {
    Send_cmd(0x06, 0x00, volume);
}

void DF_Next (void)
{
	Send_cmd(0x01, 0x00, 0x00);
}

void DF_Pause (void)
{
	Send_cmd(0x0E, 0, 0);
}

void DF_Continue (void)
{
	Send_cmd(0x0D, 0, 0);
}

void DF_Previous (void)
{
	Send_cmd(0x02, 0, 0);
}

void DF_Playback (void)
{
	Send_cmd(0x0D, 0, 0);
}

void DF_Play(uint16_t Num) {
    Send_cmd(0x12,(uint8_t)(Num << 8),(uint8_t)Num);
}

/**
 * @brief 播放指定文件夹下的歌曲
 * @note 文件夹@ref folder 需要以两位数字命名, 如00, 01, ...
 *       歌曲@ref number 需要以三位数字为前缀命名, 如000, 001, ...
 * @param folder 文件夹名(1 ~ 99)
 * @param number 歌曲名(1 ~ 255)
 */
void DF_PlayFromFolder(uint8_t folder, uint8_t number) {
    Send_cmd(0x0F, folder, number);
}

void DF_LoopFromFolder(uint8_t folder) {
    Send_cmd(0x17, 0x00, folder);
}