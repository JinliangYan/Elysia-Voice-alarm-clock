/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#include "stm32f10x.h"

#define LOG_TAG "DFPLAYER_MINI"
#include "elog.h"

#define SOURCE      0x02  // TF CARD


/*************************************** NO CHANGES AFTER THIS *************************************************/

# define START_BYTE     0x7E
# define END_BYTE       0xEF
# define VERSION        0xFF
#define  DATA_LEN       (6)
# define PACKET_LEN     (8)
# define FEEDBACK       0x00    //If need for FEEDBACK: 0x01,  No FEEDBACK: 0


/*************************************** 串口USART1 *************************************************/

typedef enum {
    UART_RECV_IDLE = 0,
    UART_RECV_VER,
    UART_RECV_LENTH,
    UART_RECV_CMD,
    UART_RECV_FEEDBACK,
    UART_RECV_DATAH,
    UART_RECV_DATAL,
    UART_RECV_CHECKSUMH,
    UART_RECV_CHECKSUML,
    UART_RECV_OVER,
} Serial_RecvStatus;

static uint8_t Serial_TxPacket[PACKET_LEN];
__attribute__((unused)) static uint8_t Serial_RxPacket[PACKET_LEN];
static Serial_RecvStatus RxState;
static uint8_t RecvOver_Flag;
static uint8_t RecvError_Flag;

static void DF_serial_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);

    log_i("The serial of DF is initialize success.");
}

static void DF_serial_SendByte(uint8_t Byte) {
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

static void DF_serial_SendArray(uint8_t *Array, uint16_t Length) {
    uint16_t i;
    for (i = 0; i < Length; i ++) {
        DF_serial_SendByte(Array[i]);
    }
}

static void DF_serial_SendPacket(void) {
    DF_serial_SendByte(START_BYTE);
    DF_serial_SendArray(Serial_TxPacket, PACKET_LEN);
    DF_serial_SendByte(END_BYTE);
    log_i("Send packet: %02X %02X %02X %02X %02X %02X %02X %02X.",
          Serial_TxPacket[0], Serial_TxPacket[1], Serial_TxPacket[2], Serial_TxPacket[3], Serial_TxPacket[4], Serial_TxPacket[5], Serial_TxPacket[6], Serial_TxPacket[7]);
}

__attribute__((unused)) void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        log_i("USART1_IRQHandler is invoked.");
        uint8_t RxData = USART_ReceiveData(USART1);
        log_i("RxData = %02X", RxData);

        switch (RxState) {
            case UART_RECV_IDLE:
                if (RxData == START_BYTE)
                    RxState = UART_RECV_VER;
                else
                    RxState = UART_RECV_IDLE;
                break;
            case UART_RECV_VER:
                Serial_RxPacket[0] = RxData;
                RxState = UART_RECV_LENTH;
                break;
            case UART_RECV_LENTH:
                Serial_RxPacket[1] = RxData;
                RxState = UART_RECV_CMD;
                break;
            case UART_RECV_CMD:
                Serial_RxPacket[2] = RxData;
                RxState = UART_RECV_FEEDBACK;
                break;
            case UART_RECV_FEEDBACK:
                Serial_RxPacket[3] = RxData;
                RxState = UART_RECV_DATAH;
                break;
            case UART_RECV_DATAH:
                Serial_RxPacket[4] = RxData;
                RxState = UART_RECV_DATAL;
                break;
            case UART_RECV_DATAL:
                Serial_RxPacket[5] = RxData;
                RxState = UART_RECV_CHECKSUMH;
                break;
            case UART_RECV_CHECKSUMH:
                Serial_RxPacket[6] = RxData;
                RxState = UART_RECV_CHECKSUML;
                break;
            case UART_RECV_CHECKSUML:
                Serial_RxPacket[7] = RxData;
                RxState = UART_RECV_OVER;
                break;
            case UART_RECV_OVER:
                if (RxData == END_BYTE) {
                    RecvOver_Flag = 1;
                    RxState = UART_RECV_IDLE;
                    log_i("Packet is received success: %02X %02X %02X %02X %02X %02X %02X %02X.",
                          Serial_RxPacket[0], Serial_RxPacket[1], Serial_RxPacket[2], Serial_RxPacket[3], Serial_RxPacket[4], Serial_RxPacket[5], Serial_RxPacket[6], Serial_RxPacket[7]);
                } else {
                    RecvError_Flag = 1;
                }
                break;
            default:
                break;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/***************************************************************************************/


static void DF_SendCmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
	uint16_t Checksum = VERSION + DATA_LEN + cmd + FEEDBACK + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

    Serial_TxPacket[0] = VERSION;
    Serial_TxPacket[1] = DATA_LEN;
    Serial_TxPacket[2] = cmd;
    Serial_TxPacket[3] = FEEDBACK;
    Serial_TxPacket[4] = Parameter1;
    Serial_TxPacket[5] = Parameter2;
    Serial_TxPacket[6] = (Checksum >> 8) & 0x00ff;
    Serial_TxPacket[7] = (Checksum & 0x00ff);

    DF_serial_SendPacket();
}


void DF_Init (uint8_t volume) // 0~30
{
    DF_serial_Init();
    DF_SendCmd(0x3F, 0x00, SOURCE);
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
    return (Serial_RxPacket[4] << 4) | Serial_RxPacket[5];
}

int DF_TEST(void) {
    extern void Elog_Init(void);
    Elog_Init();
    DF_Init(20);
    ELOG_ASSERT(DF_GetFileNumFromFolder(21) == 81);

    while (1) {

    }
    return 0;
}