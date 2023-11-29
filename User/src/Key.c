//
// Created by Jinliang on 11/27/2023.
//

#include "Key.h"
#define KEY_RCC                         RCC_APB2Periph_GPIOB
#define KEY_PORT                        GPIOB

#define KEY_PIN_1                       GPIO_Pin_0

uint8_t Key_KeyNumber;

void Key_Init(void) {
    RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {KEY_PIN_1, GPIO_Speed_50MHz, GPIO_Mode_IPD};
    GPIO_Init(KEY_PORT, &GPIO_InitStruct);
}

uint8_t Key_Get(void) {
    uint8_t temp = 0;
    temp=Key_KeyNumber;
    Key_KeyNumber=0;
    return temp;
}


static uint8_t Key_GetState() {
    uint8_t keyNumber=0;
    if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN_1) == SET){
        keyNumber = 1;
    }
    return keyNumber;
}

void Key_Loop(void) {
    static uint8_t nowState,lastState;
    lastState=nowState;				//按键状态更新
    nowState=Key_GetState();		//获取当前按键状态
    //如果上个时间点按键按下，这个时间点未按下，则是松手瞬间，以此避免消抖和松手检测
    if(lastState == 1 && nowState == 0) {
        Key_KeyNumber = 1;
    }
}