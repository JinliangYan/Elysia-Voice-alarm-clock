/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#include "stm32f10x.h"

#define LOG_TAG "DFPLAYER_MINI"
#include "elog.h"

#define Source      0x02  // TF CARD


/*************************************** NO CHANGES AFTER THIS *************************************************/

# define Start_Byte 0x7E
# define End_Byte   0xEF
# define Version    0xFF
# define Cmd_Len    0x06
# define Feedback   0x00    //If need for Feedback: 0x01,  No Feedback: 0


static void DF_serial_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

static void DF_serial_SendByte(uint8_t Byte) {
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

static void Serial_SendArray(uint8_t *Array, uint16_t Length) {
    uint16_t i;
    for (i = 0; i < Length; i ++) {
        DF_serial_SendByte(Array[i]);
    }
}

static void DF_SendCmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
    log_d("DF_SendCmd{cmd = 0x%02X, p1 = %d, p2 = %d}", cmd, Parameter1, Parameter2);
	uint16_t Checksum = Version + Cmd_Len + cmd + Feedback + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

	uint8_t CmdSequence[10] = { Start_Byte, Version, Cmd_Len, cmd, Feedback, Parameter1, Parameter2, (Checksum>>8)&0x00ff, (Checksum&0x00ff), End_Byte};

    Serial_SendArray(CmdSequence, 10);
}


void DF_Init (uint8_t volume) // 0~30
{
    DF_serial_Init();
    DF_SendCmd(0x3F, 0x00, Source);
    DF_SendCmd(0x06, 0x00, volume);
}

void DF_SetVolume(uint8_t volume) {
    DF_SendCmd(0x06, 0x00, volume);
}

void DF_Pause (void)
{
    DF_SendCmd(0x0E, 0, 0);
}

void DF_Continue (void)
{
    DF_SendCmd(0x0D, 0, 0);
}

/**
 * @brief 播放指定文件夹下的歌曲
 * @note 文件夹@ref folder 需要以两位数字命名, 如00, 01, ...
 *       歌曲@ref number 需要以三位数字为前缀命名, 如000, 001, ...
 * @param folder 文件夹名(1 ~ 99)
 * @param number 歌曲名(1 ~ 255)
 */
void DF_PlayFromFolder(uint8_t folder, uint8_t number) {
    DF_SendCmd(0x0F, folder, number);
}

void DF_LoopFromFolder(uint8_t folder) {
    DF_SendCmd(0x17, 0x00, folder);
}