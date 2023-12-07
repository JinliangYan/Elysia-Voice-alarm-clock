/**
* \file            ds1302.h
* \date            11/22/2023
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

#ifndef ElysiaVACLK_DS1302_H
#define ElysiaVACLK_DS1302_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern uint8_t ds1302_time[8]; //存放日期和时间

void ds1302_write_byte(uint8_t addr_or_data);      //DS1302 写一字节 函数
void ds1302_write_cmd(uint8_t addr, uint8_t dat);  //DS1302 写命令	  函数
uint8_t ds1302_read_byte(void);                    //DS1302 读一字节 函数
uint8_t ds1302_read_data(uint8_t addr);            //DS1302 写一字节 函数
void ds1302_bcd_to_dec(uint8_t* bcd, uint8_t times); //BCD 转 十进制 函数
void ds1302_dec_to_bcd(uint8_t* dec, uint8_t times); //十进制 转 BCD 函数
void ds1302_init(void);                           //DS1302 初始化日期和时间 函数
void ds1302_read(void);                           //DS1302 读取  日期和时间 函数

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //ElysiaVACLK_DS1302_H
