//
// Created by Jinliang on 11/23/2023.
//

#include "DS18B20.h"
#include "Delay.h"

#define DS13B20_DQ_RCC		RCC_APB2Periph_GPIOB		//时钟
#define DS1302_DQ_PORT		GPIOB						//端口
#define DS1302_DQ_PIN		GPIO_Pin_1					//引脚


#define DS1302_DQ_LOW() GPIO_ResetBits(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_HIGH() GPIO_SetBits(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_READ() GPIO_ReadInputDataBit(DS1302_DQ_PORT, DS1302_DQ_PIN)
#define DS1302_DQ_WRITE(X)  GPIO_WriteBit(DS1302_DQ_PORT, DS1302_DQ_PIN, X)

#define DS18B20_SKIP_ROM 		0xCC
#define DS18B20_CONVERT_T 		0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

void DS1302_SetDQInput(void) {
    /*上拉输入*/
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_IPU};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}
void DS1302_SetDQOutput(void) {
    /*推挽输出*/
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}

void DS18B20_Init(void) {
    RCC_APB2PeriphClockCmd(DS13B20_DQ_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {DS1302_DQ_PIN, GPIO_Speed_50MHz, GPIO_Mode_Out_PP};
    GPIO_Init(DS1302_DQ_PORT, &GPIO_InitStruct);
}

uint8_t ds18b20_OneWire_Start(void) {
    DS1302_SetDQOutput();
    uint8_t AckBit;
    DS1302_DQ_HIGH();

    DS1302_DQ_LOW();
    Delay_us(500);
    DS1302_SetDQInput();
    Delay_us(70);
    AckBit = DS1302_DQ_READ();
    Delay_us(500);
    return AckBit;
}

void ds18b20_OneWire_SendBit(uint8_t Bit) {
    DS1302_SetDQOutput();
    DS1302_DQ_LOW();

    Delay_us(10);
    DS1302_DQ_WRITE(Bit);
    Delay_us(50);
    DS1302_SetDQInput();
}

uint8_t ds18b20_OneWire_ReceiveBit() {
    DS1302_SetDQOutput();
    uint8_t Bit;
    DS1302_DQ_LOW();
    Delay_us(5);
    DS1302_SetDQInput();
    Delay_us(5);
    Bit = DS1302_DQ_READ();
    // delay 60 us
    Delay_us(60);
    return Bit;
}

void ds18b20_OneWire_SendByte(uint8_t Byte) {
    __disable_irq();
    uint8_t i;
    for (i = 0; i < 8; i++) {
        ds18b20_OneWire_SendBit(Byte & (0x01 << i));
    }
    __enable_irq();
}

uint8_t ds18b20_OneWire_ReceiveByte() {
    __disable_irq();
    uint8_t i;
    uint8_t Byte = 0x00;
    for (i = 0; i < 8; i++) {
        if (ds18b20_OneWire_ReceiveBit()) {
            Byte |= (0x01 << i);
        }
    }
    __enable_irq();
    return Byte;
}

void DS18B20_ConvertT() {
    ds18b20_OneWire_Start();
    ds18b20_OneWire_SendByte(DS18B20_SKIP_ROM);
    ds18b20_OneWire_SendByte(DS18B20_CONVERT_T);
}


float DS18B20_ReadT() {
    uint8_t LSB;
    uint8_t MSB;
    int Temp;
    float T;
    ds18b20_OneWire_Start();
    ds18b20_OneWire_SendByte(DS18B20_SKIP_ROM);
    ds18b20_OneWire_SendByte(DS18B20_READ_SCRATCHPAD);
    LSB = ds18b20_OneWire_ReceiveByte();
    MSB = ds18b20_OneWire_ReceiveByte();

    Temp = (MSB << 8) | LSB;
    T = Temp / 16.0;
    return T;
}