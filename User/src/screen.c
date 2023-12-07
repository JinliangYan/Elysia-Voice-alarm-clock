/**
* \file            screen.c
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

#include <stdio.h>
#include "clock.h"
#include "ds18b20.h"
#include "ssd1306.h"
#include "screen.h"

#define LOG_TAG "SCREEN"
#include "elog.h"

#define SSD1306_PUTS_S(str) SSD1306_Puts(str, &Font_7x10, SSD1306_COLOR_WHITE)
#define SSD1306_PUTS_M(str) SSD1306_Puts(str, &Font_11x18, SSD1306_COLOR_WHITE)
#define SSD1306_PUTS_L(str) SSD1306_Puts(str, &Font_16x26, SSD1306_COLOR_WHITE)

char* kaomoji[] = {
    "(OwO)", "(>_<)", "(QwQ)", "(^_^)", "(O.o)", "(>_<)",
};

static screen_t screen_type = SCREEN_TIME;

void
screen_init(void) {
    SSD1306_Init();
    ds18b20_init();
    screen_switch(SCREEN_TIME);
}

void
screen_switch(screen_t newType) {
    log_i("Screen type %d --> %d", screen_type, newType);
    screen_type = newType;
}

screen_t
screen_get_type(void) {
    return screen_type;
}

void
screen_update(void) {
    switch (screen_type) {
        case SCREEN_TIME:
            ds18b20_convert_t();
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            char buffer[20];

            /*状态栏部部分*/
            /*温度显示*/
            float T = ds18b20_read_t();
            sprintf(buffer, "%d", (uint8_t)T);
            SSD1306_GotoXY(0, 2);
            SSD1306_PUTS_S(buffer);
            SSD1306_PUTS_S("\'C  ");
            /*颜文字~*/
            SSD1306_PUTS_S(kaomoji[clock_second % 6]);

            /*显示时间*/
            /*显示时/分*/
            sprintf(buffer, "%02d:%02d", clock_hour, clock_minute);
            SSD1306_GotoXY(3, 16);
            SSD1306_PUTS_L(buffer);
            /*显示秒*/
            sprintf(buffer, ":%02d", clock_second);
            SSD1306_PUTS_M(buffer);

            /*显示当前的温馨提示*/
            SSD1306_GotoXY(0, SSD1306_HEIGHT - 1 - 20);
            SSD1306_PUTS_S(CLOCK_NAME);
            SSD1306_GotoXY(0, SSD1306_HEIGHT - 1 - 10);
            SSD1306_PUTS_S("want u 2 ");
            SSD1306_GotoXY(9 * 7 + 1, SSD1306_HEIGHT - 1 - 18);
            SSD1306_PUTS_M(clock_advice);
            SSD1306_DrawLine(0, 15, SSD1306_WIDTH - 1, 15, SSD1306_COLOR_WHITE);
            break;
        case SCREEN_MUSIC:
            SSD1306_GotoXY(16, SSD1306_HEIGHT / 2 - 12);
            SSD1306_Fill(SSD1306_COLOR_BLACK);
            SSD1306_PUTS_L("MUSIC");
            break;
        default: screen_switch(SCREEN_TIME);
    }
    SSD1306_UpdateScreen();
}