//
// Created by Jinliang on 11/22/2023.
//

#ifndef CLIONSTM32DEMO_DS1302_H
#define CLIONSTM32DEMO_DS1302_H

#include "stm32f10x.h"




extern uint8_t DS1302_Time[8];	//存放日期和时间

void DS1302_WriteByte(uint8_t addr_or_data);			//DS1302 写一字节 函数
void DS1302_WriteCmd(uint8_t addr, uint8_t dat);		//DS1302 写命令	  函数
uint8_t DS1302_ReadByte(void);							//DS1302 读一字节 函数
uint8_t DS1302_ReadData(uint8_t addr);					//DS1302 写一字节 函数
void DS1302_Bcd2Dec(uint8_t *bcd, uint8_t times);			//BCD 转 十进制 函数
void DS1302_Dec2Bcd(uint8_t *dec, uint8_t times);			//十进制 转 BCD 函数
void DS1302_Init(void);									//DS1302 初始化日期和时间 函数
void DS1302_Read(void);									//DS1302 读取  日期和时间 函数


#endif //CLIONSTM32DEMO_DS1302_H
