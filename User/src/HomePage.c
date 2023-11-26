//
// Created by Jinliang on 11/23/2023.
//

#include <stdio.h>
#include "HomePage.h"
#include "SSD1306.h"
#include "Clock.h"
#include "DS18B20.h"

#define SSD1306_Puts_S(str) SSD1306_Puts(str, &Font_7x10, SSD1306_COLOR_WHITE)
#define SSD1306_Puts_M(str) SSD1306_Puts(str, &Font_11x18, SSD1306_COLOR_WHITE)
#define SSD1306_Puts_L(str) SSD1306_Puts(str, &Font_16x26, SSD1306_COLOR_WHITE)

#define SSD1306_Putc_S(ch) SSD1306_Putc(ch, &Font_7x10, SSD1306_COLOR_WHITE)
#define SSD1306_Putc_M(ch) SSD1306_Putc(ch, &Font_11x18, SSD1306_COLOR_WHITE)
#define SSD1306_Putc_L(ch) SSD1306_Putc(ch, &Font_16x26, SSD1306_COLOR_WHITE)

const char *Kaomoji[] = {
        "(OwO)",
        "(>_<)",
        "(QwQ)",
        "(^_^)",
        "(O.o)",
        "(>_<)",
};

void HomePage_Init(void) {
    SSD1306_Init();
    DS18B20_Init();
    HomePage_FlashHomepage();
}

void HomePage_FlashHomepage(void) {
    DS18B20_ConvertT();
    char buffer[20];

    /*状态栏部部分*/
        /*温度显示*/
    float T = DS18B20_ReadT();
    sprintf(buffer, "%d", (uint8_t)T);
    SSD1306_GotoXY(0, 2);
    SSD1306_Puts_S(buffer);
    SSD1306_Puts_S("\'C  ");
        /*颜文字~*/
    SSD1306_Puts_S(Kaomoji[Clock_Second % 6]);

    /*显示时间*/
        /*显示时/分*/
    sprintf(buffer, "%02d:%02d", Clock_Hour, Clock_Minute);
    SSD1306_GotoXY(3, 16);
    SSD1306_Puts_L(buffer);
        /*显示秒*/
    sprintf(buffer, ":%02d", Clock_Second);
    SSD1306_Puts_M(buffer);

    /*显示当前的温馨提示*/
    SSD1306_GotoXY(0, SSD1306_HEIGHT - 1 - 20);
    SSD1306_Puts_S(CLOCK_Name);
    SSD1306_GotoXY(0, SSD1306_HEIGHT - 1 - 10);
    SSD1306_Puts_S("want u 2 ");
    SSD1306_GotoXY(9*7+1, SSD1306_HEIGHT - 1 - 18);
    SSD1306_Puts_M(Clock_Advice);
    SSD1306_DrawLine(0, 15, SSD1306_WIDTH - 1, 15, SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
}

void HomePage_Loop(void) {
    HomePage_FlashHomepage();
}