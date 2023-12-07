#include <stdio.h>
#include "clock.h"
#include "key.h"
#include "screen.h"
#include "timer3.h"
#include "voice.h"
#include "nvic.h"

#define LOG_TAG "MAIN"
#include "elog.h"

void system_init(void);
void elog_init_(void);

int
main() {
    elog_init_();

    system_init();
    while (1) {
        clock_update();
        screen_update();
    }
    return 0;
}

void
system_init(void) {
    voice_init(20);
    nvic_init();
    timer3_init();
    key_init();
    clock_init();
    screen_init();
}

void
elog_init_(void) {
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