/**
* \file            dfplayer_mini.c
* \date            12/7/2023
* \brief
*/

/*
* Copyright (c) 2023 JinLiang YAN
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* This file is part of Elysia-Voice-alarm-clock.
*
* Author:          JinLiang YAN <yanmiku0206@outlook.com>
*/

#include "stm32f10x.h"
#include "uart.h"

#define LOG_TAG "DFPLAYER_MINI"
#include "delay.h"
#include "elog.h"

#define SOURCE     0x02 /* TF CARD */

#define START_BYTE 0x7E
#define END_BYTE   0xEF
#define VERSION    0xFF
#define DATA_LEN   (6)
#define PACKET_LEN (8)
#define FEEDBACK   0x00 /* If we need for FEEDBACK: 0x01,  No FEEDBACK: 0 */

uint8_t uart_tx_packet[PACKET_LEN];
uint8_t uart_rx_packet[PACKET_LEN];

static void
df_send_packet(void) {
    uart_send_byte(START_BYTE);
    uart_send_bytes(uart_tx_packet, PACKET_LEN);
    uart_send_byte(END_BYTE);
    log_i("Send packet: %02X %02X %02X %02X %02X %02X %02X %02X.", uart_tx_packet[0], uart_tx_packet[1],
          uart_tx_packet[2], uart_tx_packet[3], uart_tx_packet[4], uart_tx_packet[5], uart_tx_packet[6],
          uart_tx_packet[7]);
}

static void
df_send_cmd(uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2) {
    uint16_t Checksum = VERSION + DATA_LEN + cmd + FEEDBACK + Parameter1 + Parameter2;
    Checksum = 0 - Checksum;

    uart_tx_packet[0] = VERSION;
    uart_tx_packet[1] = DATA_LEN;
    uart_tx_packet[2] = cmd;
    uart_tx_packet[3] = FEEDBACK;
    uart_tx_packet[4] = Parameter1;
    uart_tx_packet[5] = Parameter2;
    uart_tx_packet[6] = (Checksum >> 8) & 0x00ff;
    uart_tx_packet[7] = (Checksum & 0x00ff);

    df_send_packet();
}

void
df_init(uint8_t volume) // 0~30
{
    uart_init();
    df_send_cmd(0x3F, 0x00, SOURCE);
    /* Wait for initialization to complete */
    delay_s(2);
    df_send_cmd(0x06, 0x00, volume);
    log_i("DF mini player is initialize success.");
}

void
df_set_volume(uint8_t volume) {
    df_send_cmd(0x06, 0x00, volume);
}

void
df_pause(void) {
    df_send_cmd(0x0E, 0, 0);
}

void
df_continue(void) {
    df_send_cmd(0x0D, 0, 0);
}

/**
 * \brief       Play a song from a specified folder
 * \param       folder Folder name (1 ~ 99)
 * \note        The folder should be named with two-digit numbers, such as 00, 01, ...
 * \param       number Song name (1 ~ 255)
 * \note        The song number should be prefixed with three-digit numbers, such as 000, 001, ...
 */

void
df_play_from_folder(uint8_t folder, uint8_t number) {
    df_send_cmd(0x0F, folder, number);
}

void
df_loop_from_folder(uint8_t folder) {
    df_send_cmd(0x17, 0x00, folder);
}

uint8_t
df_get_file_num_from_folder(uint8_t folder) {
    df_send_cmd(0x4E, 0, folder);
    delay_ms(200);
    return (uart_rx_packet[5] << 4) | uart_rx_packet[6];
}

/*------------------ DEBUG --------------------------*/
int
df_test(void) {
    extern void elog_init_(void);
    elog_init_();
    log_d("df_test");
    df_init(20);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    ELOG_ASSERT(df_get_file_num_from_folder(21) == 84);
    log_d("TEST PASSED!");
    while (1) {}
    return 0;
}