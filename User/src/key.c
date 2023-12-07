/**
* \file            key.c
* \date            11/29/2023
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

#include "key.h"
#include "screen.h"
#include "voice.h"
#include "multi_button.h"

#define LOG_TAG "KEY"
#include "elog.h"

#define KEY_RCC                RCC_APB2Periph_GPIOA
#define KEY_PORT               GPIOA

// 0 有效电平按键
#define KEY_MODE_PIN           GPIO_Pin_0 //模式切换键
#define KEY_PLAY_PAUSE_PIN     GPIO_Pin_1 //播放暂停键
//#define KEY_BRIGHTNESS_PIN    GPIO_Pin_2          //亮度调节
#define KEY_VOICE_RESPONSE_PIN GPIO_Pin_2 //语音回应
#define KEY_SET_TIME_ALARM_PIN GPIO_Pin_3 //设置时间、设置语音闹钟
// 1 有效电平按键
#define KEY_VOLUME_PREV_PIN    GPIO_Pin_4 //音量递增、上一曲目
#define KEY_VOLUME_NEXT_PIN    GPIO_Pin_5 //音量递减、下一曲目
#define KEY_TIME_DECREASE_PIN  GPIO_Pin_6 //设置时间递减
#define KEY_TIME_INCREASE_PIN  GPIO_Pin_7 //设置时间递增

struct Button MODE,               //永远打开
    PLAY_PAUSE,                   //只有音乐模式打开
    VOICE_RESPONSE,               //播放不被长时间占用时打开
    SET_TIME_ALARM,               //时间模式时打开
    VOLUME_PREV, VOLUME_NEXT,     //永远打开
    TIME_DECREASE, TIME_INCREASE; //设置时间时才打开

static uint8_t
read_button_gpio(uint8_t button_id) {
    switch (button_id) {
        case KEY_MODE: return GPIO_ReadInputDataBit(KEY_PORT, KEY_MODE_PIN);
        case KEY_PLAY_PAUSE: return GPIO_ReadInputDataBit(KEY_PORT, KEY_PLAY_PAUSE_PIN);
        case KEY_VOICE_RESPONSE: return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOICE_RESPONSE_PIN);
        case KEY_SET_TIME_ALARM: return GPIO_ReadInputDataBit(KEY_PORT, KEY_SET_TIME_ALARM_PIN);
        case KEY_VOLUME_PREV: return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOLUME_PREV_PIN);
        case KEY_VOLUME_NEXT: return GPIO_ReadInputDataBit(KEY_PORT, KEY_VOLUME_NEXT_PIN);
        case KEY_TIME_DECREASE: return GPIO_ReadInputDataBit(KEY_PORT, KEY_TIME_DECREASE_PIN);
        case KEY_TIME_INCREASE: return GPIO_ReadInputDataBit(KEY_PORT, KEY_TIME_INCREASE_PIN);
        default: return 0;
    }
}

//////////////////////////////////////////////////////////////////////
/////////////////////// Key Event Handler ////////////////////////////

static void
key_update(void) {
    switch (screen_get_type()) {
        case SCREEN_MUSIC:
            button_stop(&VOICE_RESPONSE);
            button_stop(&SET_TIME_ALARM);
            button_stop(&TIME_INCREASE);
            button_stop(&TIME_DECREASE);

            button_start(&PLAY_PAUSE);

            voice_music_play();
            break;
        case SCREEN_TIME:
            button_stop(&PLAY_PAUSE);
            button_stop(&TIME_DECREASE);
            button_stop(&TIME_INCREASE);

            button_start(&VOICE_RESPONSE);
            button_start(&SET_TIME_ALARM);
            break;
        default: log_e("Illegal screen_t type");
    }
}

//MODE
static void
mode_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("Switch mode...");
    screen_switch((screen_get_type() + 1) % SCREEN_TYPE_NUM);
    key_update();
}

//PLAY_PAUSE
static void
play_pause_single_click_handler(__attribute__((unused)) void* btn) {
    static uint8_t i;
    if (i % 2 == 0) {
        log_i("Pause the music...");
        voice_music_pause();
    } else {
        log_i("Continue the music...");
        voice_music_continue();
    }
    i++;
}

//VOICE_RESPONSE
static void
voice_response_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("Say something...");
    voice_invoke();
}

//SET_TIME_ALARM
static void
set_time_single_click_start_handler(__attribute__((unused)) void* btn) {
    log_i("set_time_single_click_start_handler Invoked");
    //TODO 更换时间显示 (时间, 闹钟1, 闹钟2...)
}

static void
set_time_alarm_long_press_start_handler(__attribute__((unused)) void* btn) {
    log_i("set_time_alarm_long_press_start_handler Invoked");
    //长按开始设置时间, 再次长按保存
    static _Bool i;
    if (i == 0) {
        //TODO 进入时间设置模式
        log_i("Setting time...");
        button_start(&TIME_DECREASE);
        button_start(&TIME_INCREASE);
    } else {
        //TODO 保存时间
        log_i("Saving time...");
        button_stop(&TIME_DECREASE);
        button_stop(&TIME_INCREASE);
    }
    i++;
}

//VOLUME_PREV
static void
volume_prev_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("Decrease the volume...");
    voice_volume_decrease();
}

static void
volume_prev_long_press_start_handler(__attribute__((unused)) void* btn) {
    if (screen_get_type() != SCREEN_MUSIC) {
        return;
    }
    log_i("Previous music...");
    voice_music_previous();
}

//VOLUME_NEXT
static void
volume_next_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("Increase the volume...");
    voice_volume_increase();
}

static void
volume_next_long_press_start_handler(__attribute__((unused)) void* btn) {
    if (screen_get_type() != SCREEN_MUSIC) {
        return;
    }
    log_i("Next music...");
    voice_music_next();
}

//TIME_DECREASE
static void
time_decrease_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("time_decrease_single_click_handler Invoked");
    //TODO Decrease time
}

//TIME_INCREASE
static void
time_increase_single_click_handler(__attribute__((unused)) void* btn) {
    log_i("time_increase_single_click_handler Invoked");
    //TODO Increase time
}

//////////////////////////////////////////////////////////////////////

void
key_init(void) {
    RCC_APB2PeriphClockCmd(KEY_RCC, ENABLE);

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.GPIO_Pin = KEY_MODE_PIN | KEY_PLAY_PAUSE_PIN | KEY_VOICE_RESPONSE_PIN | KEY_SET_TIME_ALARM_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &gpio_init_struct);

    gpio_init_struct.GPIO_Pin =
        KEY_VOLUME_PREV_PIN | KEY_VOLUME_NEXT_PIN | KEY_TIME_DECREASE_PIN | KEY_TIME_INCREASE_PIN;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY_PORT, &gpio_init_struct);

    /*Initializes the button struct handle*/
    button_init(&MODE, read_button_gpio, 0, KEY_MODE);
    button_init(&PLAY_PAUSE, read_button_gpio, 0, KEY_PLAY_PAUSE);
    button_init(&VOICE_RESPONSE, read_button_gpio, 0, KEY_VOICE_RESPONSE);
    button_init(&SET_TIME_ALARM, read_button_gpio, 0, KEY_SET_TIME_ALARM);
    button_init(&VOLUME_PREV, read_button_gpio, 1, KEY_VOLUME_PREV);
    button_init(&VOLUME_NEXT, read_button_gpio, 1, KEY_VOLUME_NEXT);
    button_init(&TIME_DECREASE, read_button_gpio, 1, KEY_TIME_DECREASE);
    button_init(&TIME_INCREASE, read_button_gpio, 1, KEY_TIME_INCREASE);

    /*Attach the keys' event*/
    //MODE
    button_attach(&MODE, SINGLE_CLICK, mode_single_click_handler);
    button_attach(&MODE, PRESS_REPEAT, mode_single_click_handler);
    //PLAY_PAUSE
    button_attach(&PLAY_PAUSE, SINGLE_CLICK, play_pause_single_click_handler);
    button_attach(&PLAY_PAUSE, PRESS_REPEAT, play_pause_single_click_handler);
    //VOICE_RESPONSE
    button_attach(&VOICE_RESPONSE, SINGLE_CLICK, voice_response_single_click_handler);
    button_attach(&VOICE_RESPONSE, PRESS_REPEAT, voice_response_single_click_handler);
    //SET_TIME_ALARM
    button_attach(&SET_TIME_ALARM, SINGLE_CLICK, set_time_single_click_start_handler);
    button_attach(&SET_TIME_ALARM, PRESS_REPEAT, set_time_single_click_start_handler);
    button_attach(&SET_TIME_ALARM, LONG_PRESS_START, set_time_alarm_long_press_start_handler);
    //KEY_VOLUME_PREV
    button_attach(&VOLUME_PREV, SINGLE_CLICK, volume_prev_single_click_handler);
    button_attach(&VOLUME_PREV, PRESS_REPEAT, volume_prev_single_click_handler);
    button_attach(&VOLUME_PREV, LONG_PRESS_START, volume_prev_long_press_start_handler);
    //VOLUME_NEXT
    button_attach(&VOLUME_NEXT, SINGLE_CLICK, volume_next_single_click_handler);
    button_attach(&VOLUME_NEXT, LONG_PRESS_START, volume_next_long_press_start_handler);
    //TIME_DECREASE
    button_attach(&TIME_DECREASE, SINGLE_CLICK, time_decrease_single_click_handler);
    button_attach(&TIME_DECREASE, PRESS_REPEAT, time_decrease_single_click_handler);
    //TIME_INCREASE
    button_attach(&TIME_INCREASE, SINGLE_CLICK, time_increase_single_click_handler);
    button_attach(&TIME_INCREASE, PRESS_REPEAT, time_increase_single_click_handler);

    button_start(&MODE);
    button_start(&VOLUME_PREV);
    button_start(&VOLUME_NEXT);
    /*在此之前, Screen应该被初始化*/
    key_update();
}

//int main() {
//    printf_init();
//    key_init();
//    timer3_init();
//    voice_init(10);
//
//    printf_("%d\r\n", read_button_gpio(KEY_VOICE_RESPONSE));
//    while (1) {
//
//    }
//}

__attribute__((unused)) void
TIM3_IRQHandler(void) {
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