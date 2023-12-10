/**
* \file            ds18b20.c
* \date            11/23/2023
* \brief           Driver for DS18B20
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

#include "ds18b20.h"
#include "delay.h"

#define DS18B20_DQ_RCC          RCC_APB2Periph_GPIOB /* Clock */
#define DS18B20_DQ_PORT         GPIOB                /* Port */
#define DS18B20_DQ_PIN          GPIO_Pin_1           /* Pin */

/* Macros for controlling the DS18B20 data line (DQ) */
#define DS18B20_DQ_LOW()        GPIO_ResetBits(DS18B20_DQ_PORT, DS18B20_DQ_PIN)
#define DS18B20_DQ_HIGH()       GPIO_SetBits(DS18B20_DQ_PORT, DS18B20_DQ_PIN)
#define DS18B20_DQ_READ()       GPIO_ReadInputDataBit(DS18B20_DQ_PORT, DS18B20_DQ_PIN)
#define DS18B20_DQ_WRITE(X)     GPIO_WriteBit(DS18B20_DQ_PORT, DS18B20_DQ_PIN, X)

/* DS18B20 commands */
#define DS18B20_SKIP_ROM        0xCC
#define DS18B20_CONVERT_T       0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

/**
 * \brief Set the DQ pin as pull-up input.
 */
void
gpio_set_dq_input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {DS18B20_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_IPU};
    GPIO_Init(DS18B20_DQ_PORT, &GPIO_InitStruct);
}

/**
 * \brief Set the DQ pin as pull-up output.
 */
void
gpio_set_dq_output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {DS18B20_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS18B20_DQ_PORT, &GPIO_InitStruct);
}

/**
 * \brief Initialize DS18B20
 */
void
ds18b20_init(void) {
    RCC_APB2PeriphClockCmd(DS18B20_DQ_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {DS18B20_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS18B20_DQ_PORT, &GPIO_InitStruct);
}

/**
 * \brief Start the one-wire communication for DS18B20 sensor
 *
 * This function initiates communication with the DS18B20 sensor using the one-wire protocol.
 * It sends a start signal, waits for the acknowledgment, and returns the acknowledgment status.
 *
 * \return Acknowledgment status:
 *         - `0`: Acknowledgment received
 *         - `1`: Acknowledgment not received
 */
bool
ds18b20_one_wire_start(void) {
    gpio_set_dq_output();
    bool AckBit;
    DS18B20_DQ_HIGH();

    DS18B20_DQ_LOW();
    delay_us(500);
    gpio_set_dq_input();
    delay_us(70);
    AckBit = DS18B20_DQ_READ();
    delay_us(500);
    return AckBit;
}

/**
 * \brief Send a single bit over the one-wire communication for DS18B20 sensor
 *
 * This function sends a single bit over the one-wire communication for DS18B20 sensor.
 *
 * \param Bit: The bit to be sent (0 or 1)
 */
void
ds18b20_one_wire_send_bit(uint8_t Bit) {
    gpio_set_dq_output();
    DS18B20_DQ_LOW();

    delay_us(10);
    DS18B20_DQ_WRITE(Bit);
    delay_us(50);
    gpio_set_dq_input();
}

/**
 * \brief Receive a single bit over the one-wire communication for DS18B20 sensor
 * \return The received bit (0 or 1)
 */
uint8_t
ds18b20_one_wire_receive_bit() {
    gpio_set_dq_output();
    uint8_t Bit;
    DS18B20_DQ_LOW();
    delay_us(5);
    gpio_set_dq_input();
    delay_us(5);
    Bit = DS18B20_DQ_READ();
    delay_us(60);
    return Bit;
}

/**
 * \brief Send a byte over the one-wire communication for DS18B20 sensor
 * \param Byte: The byte to be sent
 */
void
ds18b20_one_wire_send_byte(uint8_t Byte) {
    /* Disable interrupts after the byte transmission */
    __disable_irq();
    uint8_t i;
    for (i = 0; i < 8; i++) {
        ds18b20_one_wire_send_bit(Byte & (0x01 << i));
    }
    /* Enable interrupts after the byte transmission */
    __enable_irq();
}

/**
 * \brief Receive a byte over the one-wire communication for DS18B20 sensor
 * \return The received byte
 */
uint8_t
ds18b20_one_wire_receive_byte() {
    /* Disable interrupts during the byte reception */
    __disable_irq();
    uint8_t i;
    uint8_t Byte = 0x00;
    for (i = 0; i < 8; i++) {
        if (ds18b20_one_wire_receive_bit()) {
            Byte |= (0x01 << i);
        }
    }
    /* Enable interrupts after the byte reception */
    __enable_irq();
    return Byte;
}

/**
 * \brief Initiate temperature conversion for DS18B20 sensor
 */
void
ds18b20_convert_t() {
    ds18b20_one_wire_start();
    ds18b20_one_wire_send_byte(DS18B20_SKIP_ROM);
    ds18b20_one_wire_send_byte(DS18B20_CONVERT_T);
}

/**
 * \brief Read temperature value from DS18B20 sensor
 * \return Temperature value in degrees Celsius
 */
float
ds18b20_read_t() {
    uint8_t LSB;
    uint8_t MSB;
    int Temp;
    float T;
    ds18b20_one_wire_start();
    ds18b20_one_wire_send_byte(DS18B20_SKIP_ROM);
    ds18b20_one_wire_send_byte(DS18B20_READ_SCRATCHPAD);
    LSB = ds18b20_one_wire_receive_byte();
    MSB = ds18b20_one_wire_receive_byte();

    Temp = (MSB << 8) | LSB;
    T = Temp / 16.0;
    return T;
}