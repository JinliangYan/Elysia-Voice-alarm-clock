#include <stdio.h>
#include "nvic.h"
#include "Screen.h"
#include "Clock.h"
#include "Voice.h"
#include "Key.h"
#include "Timer3.h"

#define LOG_TAG "MAIN"
#include "elog.h"

void System_Init(void);
void Elog_Init(void);
int main() {
    Elog_Init();

    System_Init();
    while (1) {
        Clock_Update();
        Screen_Update();
    }
    return 0;
}

void System_Init(void) {
    Voice_Init(20);
    nvic_init();
    Timer3_Init();
    Key_Init();
    Clock_Init();
    Screen_Init();
}

void Elog_Init(void) {
    /* close printf buffer */
    setbuf(stdout, NULL);
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    /* start EasyLogger */
    elog_start();
}