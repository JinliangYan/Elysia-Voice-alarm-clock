/**
* \file            ds18b20.c
* \date            11/23/2023
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

#include "ds18b20.h"
#include "delay.h"

#define DS13B20_DQ_RCC          RCC_APB2Periph_GPIOB //时钟
#define DS1302_DQ_PORT          GPIOB                //端口
#define DS1302_DQ_PIN           GPIO_Pin_1           //引脚

#define DS1302_DQ_LOW()         GPIO_ResetBits(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_HIGH()        GPIO_SetBits(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_READ()        GPIO_ReadInputDataBit(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_WRITE(X)      GPIO_WriteBit(DS1302_DQ_PORT, DS1302_DQ_PIN, X)

#define DS18B20_SKIP_ROM        0xCC
#define DS18B20_CONVERT_T       0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

void
gpio_set_dq_input(void) {
    /*上拉输入*/
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_IPU};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}

void
gpio_set_dq_output(void) {
    /*推挽输出*/
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}

void
ds18b20_init(void) {
    RCC_APB2PeriphClockCmd(DS13B20_DQ_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}

uint8_t
ds18b20_one_wire_start(void) {
    gpio_set_dq_output();
    uint8_t AckBit;
    DS1302_DQ_HIGH();

    DS1302_DQ_LOW();
    delay_us(500);
    gpio_set_dq_input();
    delay_us(70);
    AckBit = DS1302_DQ_READ();
    delay_us(500);
    return AckBit;
}

void
ds18b20_one_wire_send_bit(uint8_t Bit) {
    gpio_set_dq_output();
    DS1302_DQ_LOW();

    delay_us(10);
    DS1302_DQ_WRITE(Bit);
    delay_us(50);
    gpio_set_dq_input();
}

uint8_t
ds18b20_one_wire_receive_bit() {
    gpio_set_dq_output();
    uint8_t Bit;
    DS1302_DQ_LOW();
    delay_us(5);
    gpio_set_dq_input();
    delay_us(5);
    Bit = DS1302_DQ_READ();
    // delay 60 us
    delay_us(60);
    return Bit;
}

void
ds18b20_one_wire_send_byte(uint8_t Byte) {
    __disable_irq();
    uint8_t i;
    for (i = 0; i < 8; i++) {
        ds18b20_one_wire_send_bit(Byte & (0x01 << i));
    }
    __enable_irq();
}

uint8_t
ds18b20_one_wire_receive_byte() {
    __disable_irq();
    uint8_t i;
    uint8_t Byte = 0x00;
    for (i = 0; i < 8; i++) {
        if (ds18b20_one_wire_receive_bit()) {
            Byte |= (0x01 << i);
        }
    }
    __enable_irq();
    return Byte;
}

void
ds18b20_convert_t() {
    ds18b20_one_wire_start();
    ds18b20_one_wire_send_byte(DS18B20_SKIP_ROM);
    ds18b20_one_wire_send_byte(DS18B20_CONVERT_T);
}

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