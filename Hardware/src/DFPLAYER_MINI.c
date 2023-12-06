/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#include "stm32f10x.h"
#include "uart.h"

#define LOG_TAG "DFPLAYER_MINI"
#include "elog.h"
#include "Delay.h"

#define SOURCE      0x02  // TF CARD


/*************************************** NO CHANGES AFTER THIS *************************************************/

# define START_BYTE     0x7E
# define END_BYTE       0xEF
# define VERSION        0xFF
#define  DATA_LEN       (6)
# define PACKET_LEN     (8)
# define FEEDBACK       0x00    //If need for FEEDBACK: 0x01,  No FEEDBACK: 0


uint8_t uart_tx_packet[PACKET_LEN];
uint8_t uart_rx_packet[PACKET_LEN];

static void DF_SendPacket(void) {
    uart_send_byte(START_BYTE);
    uart_send_bytes(uart_tx_packet, PACKET_LEN);
    uart_send_byte(END_BYTE);
    log_i("Send packet: %02X %02X %02X %02X %02X %02X %02X %02X.",
          uart_tx_packet[0], uart_tx_packet[1], uart_tx_packet[2], uart_tx_packet[3], uart_tx_packet[4], uart_tx_packet[5], uart_tx_packet[6], uart_tx_packet[7]);
}

static void DF_SendCmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
	uint16_t Checksum = VERSION + DATA_LEN + cmd + FEEDBACK + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

    uart_tx_packet[0] = VERSION;
    uart_tx_packet[1] = DATA_LEN;
    uart_tx_packet[2] = cmd;
    uart_tx_packet[3] = FEEDBACK;
    uart_tx_packet[4] = Parameter1;
    uart_tx_packet[5] = Parameter2;
    uart_tx_packet[6] = (Checksum >> 8) & 0x00ff;
    uart_tx_packet[7] = (Checksum & 0x00ff);

    DF_SendPacket();
}


void DF_Init (uint8_t volume) // 0~30
{
    uart_init();
    DF_SendCmd(0x3F, 0x00, SOURCE);
    /* Wait for initialization to complete */
    Delay_s(2);
    DF_SendCmd(0x06, 0x00, volume);
    log_i("DF mini player is initialize success.");
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


uint8_t DF_GetFileNumFromFolder(uint8_t folder) {
    DF_SendCmd(0x4E, 0, folder);
    Delay_ms(200);
    return (uart_rx_packet[5] << 4) | uart_rx_packet[6];
}

/*------------------ DEBUG --------------------------*/
int DF_TEST(void) {
    extern void Elog_Init(void);
    Elog_Init();
    log_i("DF_TEST");
    DF_Init(20);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 84);
    while (1) {

    }
    return 0;
}