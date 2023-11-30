//
// Created by Jinliang on 11/29/2023.
//

#include "Key.h"
#include "multi_button.h"
#include "Voice.h"
#include "Screen.h"

#define LOG_TAG "KEY"
#include "elog.h"

#define KEY_RCC                         RCC_APB2Periph_GPIOA
#define KEY_PORT                        GPIOA

// 0 有效电平按键
#define KEY_MODE_PIN            GPIO_Pin_0          //模式切换键
#define KEY_POWER_PLAY_PIN      GPIO_Pin_1          //开关机键/播放暂停键
//#define KEY_BRIGHTNESS_PIN      GPIO_Pin_2          //亮度调节
#define KEY_VOICE_RESPONSE_PIN      GPIO_Pin_2          //语音回应
#define KEY_SET_TIME_ALARM_PIN  GPIO_Pin_3          //设置时间、设置语音闹钟
// 1 有效电平按键
#define KEY_VOLUME_PREV_PIN     GPIO_Pin_4          //音量递增、上一曲目
#define KEY_VOLUME_NEXT_PIN     GPIO_Pin_5          //音量递减、下一曲目
#define KEY_TIME_DECREASE_PIN   GPIO_Pin_6          //设置时间递减
#define KEY_TIME_INCREASE_PIN   GPIO_Pin_7          //设置时间递增

struct Button MODE, POWER_PLAY, VOICE_RESPONSE, SET_TIME_ALARM,
        VOLUME_PREV, VOLUME_NEXT, TIME_DECREASE, TIME_INCREASE;

static uint8_t read_button_GPIO(uint8_t button_id)
{
    switch(button_id)
    {
        case KEY_MODE:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_MODE_PIN);
        case KEY_POWER_PLAY:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_POWER_PLAY_PIN);
        case KEY_VOICE_RESPONSE:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOICE_RESPONSE_PIN);
        case KEY_SET_TIME_ALARM:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_SET_TIME_ALARM_PIN);
        case KEY_VOLUME_PREV:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOLUME_PREV_PIN);
        case KEY_VOLUME_NEXT:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOLUME_NEXT_PIN);
        case KEY_TIME_DECREASE:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_TIME_DECREASE_PIN);
        case KEY_TIME_INCREASE:
            return GPIO_ReadInputDataBit(KEY_PORT, KEY_TIME_INCREASE_PIN);
        default:
            return 0;
    }
}
//////////////////////////////////////////////////////////////////////
/////////////////////// Key Event Handler ////////////////////////////

//MODE
static void MODE_SINGLE_CLICK_Handler(void *btn) {
    log_i("MODE_SINGLE_CLICK_Handler Invoked");
    //TODO Turn on/off Bluetooth when single click
    Screen_Switch(Screen_GetType() % SCREEN_TYPE_NUM);
}

//POWER_PLAY
static void KEY_POWER_PLAY_SINGLE_CLICK_Handler(void *btn) {
    log_i("KEY_POWER_PLAY_SINGLE_CLICK_Handler Invoked");
    //TODO Play/pause music when single click
}
static void KEY_POWER_PLAY_LONG_PRESS_START_Handler(void *btn) {
    log_i("KEY_POWER_PLAY_LONG_PRESS_START_Handler Invoked");
    //TODO Turn on/off power instantly when long press start
}

//VOICE_RESPONSE
static void VOICE_RESPONSE_SINGLE_CLICK_Handler(void *btn) {
    log_i("VOICE_RESPONSE_SINGLE_CLICK_Handler Invoked");
    Voice_Invoke();
}

//SET_TIME_ALARM
static void KEY_SET_TIME_SINGLE_CLICK_START_Handler(void *btn) {
    log_i("KEY_SET_TIME_SINGLE_CLICK_START_Handler Invoked");
    //TODO Switch the screen display to time/alarm clock 1/alarm clock 2... When single click
}
static void KEY_SET_TIME_ALARM_LONG_PRESS_START_Handler(void *btn) {
    log_i("KEY_SET_TIME_ALARM_LONG_PRESS_START_Handler Invoked");
    //TODO Current screen time setting when long press start
    // Save time changes when long press start again
}

//VOLUME_PREV
static void KEY_VOLUME_PREV_SINGLE_CLICK_Handler(void *btn) {
    log_i("KEY_VOLUME_PREV_SINGLE_CLICK_Handler Invoked");
    Voice_VolumeDecrease();
}
static void KEY_VOLUME_PREV_LONG_PRESS_START_Handler(void *btn) {
    log_i("KEY_VOLUME_PREV_LONG_PRESS_START_Handler Invoked");
    //TODO Previous music
}

//VOLUME_NEXT
static void KEY_VOLUME_NEXT_SINGLE_CLICK_Handler(void *btn) {
    log_i("KEY_VOLUME_NEXT_SINGLE_CLICK_Handler Invoked");
    //TODO Increase volume
    Voice_VolumeIncrease();
}
static void KEY_VOLUME_NEXT_LONG_PRESS_START_Handler(void *btn) {
    log_i("KEY_VOLUME_NEXT_LONG_PRESS_START_Handler Invoked");
    //TODO Next music
}

//TIME_DECREASE
static void KEY_TIME_DECREASE_SINGLE_CLICK_Handler(void *btn) {
    log_i("KEY_TIME_DECREASE_SINGLE_CLICK_Handler Invoked");
    //TODO Decrease time
}

//TIME_INCREASE
static void KEY_TIME_INCREASE_SINGLE_CLICK_Handler(void *btn) {
    log_i("KEY_TIME_INCREASE_SINGLE_CLICK_Handler Invoked");
    //TODO Increase time
}

//////////////////////////////////////////////////////////////////////

void Key_Init(void) {
    RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = KEY_MODE_PIN | KEY_POWER_PLAY_PIN | KEY_VOICE_RESPONSE_PIN | KEY_SET_TIME_ALARM_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = KEY_VOLUME_PREV_PIN | KEY_VOLUME_NEXT_PIN | KEY_TIME_DECREASE_PIN | KEY_TIME_INCREASE_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY_PORT, &GPIO_InitStruct);

    /*Initializes the button struct handle*/
    button_init(&MODE, read_button_GPIO, 0, KEY_MODE);
    button_init(&POWER_PLAY, read_button_GPIO, 0, KEY_POWER_PLAY);
    button_init(&VOICE_RESPONSE, read_button_GPIO, 0, KEY_VOICE_RESPONSE);
    button_init(&SET_TIME_ALARM, read_button_GPIO, 0, KEY_SET_TIME_ALARM);
    button_init(&VOLUME_PREV, read_button_GPIO, 1, KEY_VOLUME_PREV);
    button_init(&VOLUME_NEXT, read_button_GPIO, 1, KEY_VOLUME_NEXT);
    button_init(&TIME_DECREASE, read_button_GPIO, 1, KEY_TIME_DECREASE);
    button_init(&TIME_INCREASE, read_button_GPIO, 1, KEY_TIME_INCREASE);

    /*Attach the keys' event*/
    //MODE
    button_attach(&MODE, SINGLE_CLICK, MODE_SINGLE_CLICK_Handler);
    button_attach(&MODE, PRESS_REPEAT, MODE_SINGLE_CLICK_Handler);
    //POWER_PLAY
    button_attach(&POWER_PLAY, SINGLE_CLICK, KEY_POWER_PLAY_SINGLE_CLICK_Handler);
    button_attach(&POWER_PLAY, PRESS_REPEAT, KEY_POWER_PLAY_SINGLE_CLICK_Handler);
    button_attach(&POWER_PLAY, LONG_PRESS_START, KEY_POWER_PLAY_LONG_PRESS_START_Handler);
    //VOICE_RESPONSE
    button_attach(&VOICE_RESPONSE, SINGLE_CLICK, VOICE_RESPONSE_SINGLE_CLICK_Handler);
    button_attach(&VOICE_RESPONSE, PRESS_REPEAT, VOICE_RESPONSE_SINGLE_CLICK_Handler);
    //SET_TIME_ALARM
    button_attach(&SET_TIME_ALARM, SINGLE_CLICK, KEY_SET_TIME_SINGLE_CLICK_START_Handler);
    button_attach(&SET_TIME_ALARM, PRESS_REPEAT, KEY_SET_TIME_SINGLE_CLICK_START_Handler);
    button_attach(&SET_TIME_ALARM, LONG_PRESS_START, KEY_SET_TIME_ALARM_LONG_PRESS_START_Handler);
    //KEY_VOLUME_PREV
    button_attach(&VOLUME_PREV, SINGLE_CLICK, KEY_VOLUME_PREV_SINGLE_CLICK_Handler);
    button_attach(&VOLUME_PREV, PRESS_REPEAT, KEY_VOLUME_PREV_SINGLE_CLICK_Handler);
    button_attach(&VOLUME_PREV, LONG_PRESS_START, KEY_VOLUME_PREV_LONG_PRESS_START_Handler);
    //VOLUME_NEXT
    button_attach(&VOLUME_NEXT, SINGLE_CLICK, KEY_VOLUME_NEXT_SINGLE_CLICK_Handler);
    button_attach(&VOLUME_NEXT, LONG_PRESS_START, KEY_VOLUME_NEXT_LONG_PRESS_START_Handler);
    //TIME_DECREASE
    button_attach(&TIME_DECREASE, SINGLE_CLICK, KEY_TIME_DECREASE_SINGLE_CLICK_Handler);
    button_attach(&TIME_DECREASE, PRESS_REPEAT, KEY_TIME_DECREASE_SINGLE_CLICK_Handler);
    //TIME_INCREASE
    button_attach(&TIME_INCREASE, SINGLE_CLICK, KEY_TIME_INCREASE_SINGLE_CLICK_Handler);
    button_attach(&TIME_INCREASE, PRESS_REPEAT, KEY_TIME_INCREASE_SINGLE_CLICK_Handler);

    button_start(&MODE);
    button_start(&POWER_PLAY);
    button_start(&VOICE_RESPONSE);
    button_start(&SET_TIME_ALARM);
    button_start(&VOLUME_PREV);
    button_start(&VOLUME_NEXT);
    button_start(&TIME_DECREASE);
    button_start(&TIME_INCREASE);
}


//int main() {
//    printf_init();
//    Key_Init();
//    Timer3_Init();
//    Voice_Init(10);
//
//    printf_("%d\r\n", read_button_GPIO(KEY_VOICE_RESPONSE));
//    while (1) {
//
//    }
//}


__attribute__((unused)) void TIM3_IRQHandler(void) {
    static uint8_t counter1, counter2;
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        counter1++;
        counter2++;
        if (counter1 == TICKS_INTERVAL) {
            counter1 = 0;
            button_ticks();
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}