//
// Created by Jinliang on 11/22/2023.
//

#ifndef ElysiaVACLK_DS1302_H
#define ElysiaVACLK_DS1302_H

#include "stm32f10x.h"

extern uint8_t ds1302_time[8]; //存放日期和时间

void ds1302_write_byte(uint8_t addr_or_data);      //DS1302 写一字节 函数
void ds1302_write_cmd(uint8_t addr, uint8_t dat);  //DS1302 写命令	  函数
uint8_t ds1302_read_byte(void);                    //DS1302 读一字节 函数
uint8_t ds1302_read_data(uint8_t addr);            //DS1302 写一字节 函数
void ds1302_bcd_to_dec(uint8_t* bcd, uint8_t times); //BCD 转 十进制 函数
void ds1302_dec_to_bcd(uint8_t* dec, uint8_t times); //十进制 转 BCD 函数
void ds1302_init(void);                           //DS1302 初始化日期和时间 函数
void ds1302_read(void);                           //DS1302 读取  日期和时间 函数

#endif //ElysiaVACLK_DS1302_H
