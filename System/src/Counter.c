//
// Created by Jinliang on 11/12/2023.
//

#include "Counter.h"

void Counter_Init(void) {
    //开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //选择时基单元的时钟
    TIM_InternalClockConfig(TIM2);

    //初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;    //基本定时器无，随便设为0
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    //启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

uint16_t Counter_Get(void) {
    return TIM_GetCounter(TIM2);
}

void Counter_Reset(void) {
    TIM_SetCounter(TIM2, 0);
}