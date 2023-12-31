/**
 * \file            clock_cfg.h
 * \date            12/10/2023
 * \brief           Clock configuration file
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

#ifndef ELYSIA_VOICE_ALARM_CLOCK_CLOCK_CFG_H
#define ELYSIA_VOICE_ALARM_CLOCK_CLOCK_CFG_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \brief          The clock name
 * \hideinitializer
 */
#define CLOCK_CFG_CLOCK_NAME "Elysia"

/**
 * \brief          User birthday configuration
 * \hideinitializer
 */
#define CLOCK_CFG_BIRTHDAY "02-06"

/**
 * \brief          User sleep time configuration
 * \hideinitializer
 */
#define CLOCK_CFG_SLEEP_TIME "11:30"

/**
 * \brief          User getup time configuration
 * \hideinitializer
 */
#define CLOCK_CFG_GETUP_TIME "07:30"

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ELYSIA_VOICE_ALARM_CLOCK_CLOCK_CFG_H */
