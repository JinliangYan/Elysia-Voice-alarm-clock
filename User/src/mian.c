/**
* \file            main.c
* \date            11/23/2023
* \brief           Main entry point for the Elysia-Voice-alarm-clock application.
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
#include "key.h"
#include "screen.h"
#include "timer3.h"
#include "voice.h"
#include "nvic.h"

#define LOG_TAG "MAIN"
#include "elog.h"

/**
* \brief           System initialization function, initializing all necessary modules.
*/
void system_init(void);

/**
* \brief           EasyLogger initialization function.
*/
void elog_init_(void);

/**
* \brief           Main entry point for the application.
* \return          Exit status of the program.
*/
int main() {
   elog_init_();

   system_init();
   while (1) {
       clock_update();
       screen_update();
   }
   return 0;
}

/**
* \brief           System initialization function, initializing voice, NVIC, timer, key, clock, and screen modules.
*/
void system_init(void) {
   voice_init(20);
   nvic_init();
   timer3_init();
   key_init();
   clock_init();
   screen_init();
}

/**
* \brief           EasyLogger initialization function.
*/
void elog_init_(void) {
   /* Close printf buffer */
   setbuf(stdout, NULL);
   /* Initialize EasyLogger */
   elog_init();
   /* Set EasyLogger log format */
   elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
   elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
   elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
   elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
   elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
   elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
   /* Start EasyLogger */
   elog_start();
}
