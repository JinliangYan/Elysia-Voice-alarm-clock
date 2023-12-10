/**
* \file            ds1302.c
* \date            11/22/2023
* \brief           DS1302 RTC Driver Implementation
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

#include "ds1302.h"

/* DS1302 RTC Clock GPIO Configuration */
#define DS1302_CLK_RCC       RCC_APB2Periph_GPIOB /* Clock RCC */
#define DS1302_CLK_PORT      GPIOB                /* Clock Port */
#define DS1302_CLK_PIN       GPIO_Pin_13          /* Clock Pin */

/* DS1302 RTC Data GPIO Configuration */
#define DS1302_DAT_RCC       RCC_APB2Periph_GPIOB /* Data RCC */
#define DS1302_DAT_PORT      GPIOB                /* Data Port */
#define DS1302_DAT_PIN       GPIO_Pin_14          /* Data Pin */

/* DS1302 RTC Reset GPIO Configuration */
#define DS1302_RST_RCC       RCC_APB2Periph_GPIOB /* Reset RCC */
#define DS1302_RST_PORT      GPIOB                /* Reset Port */
#define DS1302_RST_PIN       GPIO_Pin_15          /* Reset Pin */

/* DS1302 RTC Clock High */
#define DS1302_CLK_HIGH      GPIO_SetBits(DS1302_CLK_PORT, DS1302_CLK_PIN)   /* Clock Pin High */
#define DS1302_CLK_LOW       GPIO_ResetBits(DS1302_CLK_PORT, DS1302_CLK_PIN) /* Clock Pin Low */

/* DS1302 RTC Data High */
#define DS1302_DAT_HIGH      GPIO_SetBits(DS1302_DAT_PORT, DS1302_DAT_PIN)   /* Data Pin High */
#define DS1302_DAT_LOW       GPIO_ResetBits(DS1302_DAT_PORT, DS1302_DAT_PIN) /* Data Pin Low */

/* DS1302 RTC Reset High */
#define DS1302_RST_HIGH      GPIO_SetBits(DS1302_RST_PORT, DS1302_RST_PIN)   /* Reset Pin High */
#define DS1302_RST_LOW       GPIO_ResetBits(DS1302_RST_PORT, DS1302_RST_PIN) /* Reset Pin Low */

/* Read-related register addresses */
#define DS1302_READ_SECOND   0x81 /* Seconds */
#define DS1302_READ_MINUTE   0x83 /* Minutes */
#define DS1302_READ_HOUR     0x85 /* Hours */
#define DS1302_READ_DAY      0x87 /* Day */
#define DS1302_READ_MONTH    0x89 /* Month */
#define DS1302_READ_WEEK     0x8B /* Week */
#define DS1302_READ_YEAR     0x8D /* Year */

/* Write-related register addresses */
#define DS1302_WRITE_SECOND  0x80 /* Seconds */
#define DS1302_WRITE_MINUTE  0x82 /* Minutes */
#define DS1302_WRITE_HOUR    0x84 /* Hours */
#define DS1302_WRITE_DAY     0x86 /* Day */
#define DS1302_WRITE_MONTH   0x88 /* Month */
#define DS1302_WRITE_WEEK    0x8A /* Week */
#define DS1302_WRITE_YEAR    0x8C /* Year */
#define DS1302_WRITE_PROTECT 0x8E /* Protect */

/* Date and time definition: Year Month Day Hour Minute Second Week */
uint8_t ds1302_time[8] = {23, 11, 23, 22, 2, 20, 4};

/**
 * \brief Set DS1302 data pin to input mode
 *
 * This function configures the DS1302 data pin to operate in input mode.
 * It initializes the corresponding GPIO pin and sets it to pull-up input mode.
 */
void
ds1302_set_input_mode(void) {
    /* Define structure members */
    GPIO_InitTypeDef DS1302_Structure;

    /* Enable clock for the port */
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);

    /* Specify the pin and set to pull-up input mode */
    DS1302_Structure.GPIO_Pin = DS1302_DAT_PIN;
    DS1302_Structure.GPIO_Mode = GPIO_Mode_IPU;

    /* Initialize the pin with the specified configuration */
    GPIO_Init(DS1302_DAT_PORT, &DS1302_Structure);
}

/**
 * \brief Set DS1302 data pin to output mode
 *
 * This function configures the DS1302 data pin to operate in output mode.
 * It initializes the corresponding GPIO pin and sets it to push-pull output mode with a speed of 50MHz.
 */
void
ds1302_set_output_mode(void) {
    /* Define structure members */
    GPIO_InitTypeDef DS1302_Structure;

    /* Enable clock for the port */
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);

    /* Specify the pin, set to push-pull output mode, and set the speed */
    DS1302_Structure.GPIO_Pin = DS1302_DAT_PIN;
    DS1302_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
    DS1302_Structure.GPIO_Speed = GPIO_Speed_50MHz;

    /* Initialize the pin with the specified configuration */
    GPIO_Init(DS1302_DAT_PORT, &DS1302_Structure);
}

/**
 * \brief Configure DS1302 pins for communication
 *
 * This function configures the DS1302 clock (CLK), data (DAT), and reset (RST) pins for communication.
 * It initializes the corresponding GPIO pins and sets them to push-pull output mode with a speed of 50MHz.
 */
void
ds1302_config(void) {
    GPIO_InitTypeDef DS1302_Structure; /* Define structure members */

    /* Enable clock for CLK, DAT, and RST ports */
    RCC_APB2PeriphClockCmd(DS1302_CLK_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(DS1302_DAT_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(DS1302_RST_RCC, ENABLE);

    /* DS1302 CLK */
    DS1302_Structure.GPIO_Pin = DS1302_CLK_PIN;     /* Specify the pin */
    DS1302_Structure.GPIO_Mode = GPIO_Mode_Out_PP;  /* Set to push-pull output mode */
    DS1302_Structure.GPIO_Speed = GPIO_Speed_50MHz; /* Set the speed */
    GPIO_Init(DS1302_CLK_PORT, &DS1302_Structure);  /* Initialize the pin with the specified configuration */

    /* DS1302 DAT */
    DS1302_Structure.GPIO_Pin = DS1302_DAT_PIN;     /* Specify the pin */
    DS1302_Structure.GPIO_Mode = GPIO_Mode_Out_PP;  /* Set to push-pull output mode */
    DS1302_Structure.GPIO_Speed = GPIO_Speed_50MHz; /* Set the speed */
    GPIO_Init(DS1302_DAT_PORT, &DS1302_Structure);  /* Initialize the pin with the specified configuration */

    /* DS1302 RST */
    DS1302_Structure.GPIO_Pin = DS1302_RST_PIN;     /* Specify the pin */
    DS1302_Structure.GPIO_Mode = GPIO_Mode_Out_PP;  /* Set to push-pull output mode */
    DS1302_Structure.GPIO_Speed = GPIO_Speed_50MHz; /* Set the speed */
    GPIO_Init(DS1302_RST_PORT, &DS1302_Structure);  /* Initialize the pin with the specified configuration */
}

/**
 * \brief Write a byte to DS1302
 *
 * This function writes a byte to the DS1302 module.
 * It sets the DS1302 data pin to output mode and transmits the byte bit by bit, starting from the least significant bit (LSB).
 * Clock pulses are used to signal the transmission of each bit.
 *
 * \param[in] addr_or_data: Byte to be written
 */
void
ds1302_write_byte(uint8_t addr_or_data) {
    uint8_t i;
    ds1302_set_output_mode();

    for (i = 0; i < 8; i++) {
        if (addr_or_data & 0x01) {
            DS1302_DAT_HIGH; /* Set data pin high if the current bit is 1 */
        } else {
            DS1302_DAT_LOW; /* Set data pin low if the current bit is 0 */
        }

        addr_or_data >>= 1; /* Right shift to prepare the next bit */
        DS1302_CLK_HIGH;    /* Raise clock signal to indicate data transmission */
        DS1302_CLK_LOW;     /* Lower clock signal to prepare for the next bit */
    }
}

/**
 * \brief Write a command to DS1302
 *
 * This function writes a command to the DS1302 module.
 * It sets the DS1302 reset (RST) and clock (CLK) pins, writes the address, writes the data, and then resets the RST pin.
 *
 * \param[in] addr: Address to be written
 * \param[in] dat: Data to be written
 */
void
ds1302_write_cmd(uint8_t addr, uint8_t dat) {
    DS1302_RST_LOW; /* Lower the reset pin */
    DS1302_CLK_LOW; /* Lower the clock pin */

    DS1302_RST_HIGH;         /* Raise the reset pin */
    ds1302_write_byte(addr); /* Write the address */
    ds1302_write_byte(dat);  /* Write the data */

    DS1302_RST_LOW; /* Lower the reset pin to complete the command */
}

/**
 * \brief Read a byte from DS1302
 *
 * This function reads a byte from the DS1302 module.
 * It sets the DS1302 data pin to input mode and reads the byte bit by bit, starting from the most significant bit (MSB).
 * Clock pulses are used to signal the reception of each bit.
 *
 * \return The byte read from DS1302
 */
uint8_t
ds1302_read_byte(void) {
    uint8_t i;
    uint8_t dat = 0;
    ds1302_set_input_mode();

    for (i = 0; i < 8; i++) {
        dat >>= 1; /* Left shift once (reading from the most significant bit) */

        if (GPIO_ReadInputDataBit(DS1302_DAT_PORT, DS1302_DAT_PIN) == SET) {
            dat |= 0x80; /* Set the most significant bit if the current bit is 1 */
        }

        DS1302_CLK_HIGH;
        DS1302_CLK_LOW;
    }

    return dat;
}

/**
 * \brief Read data from a specific address in DS1302
 *
 * This function reads data from a specific address in the DS1302 module.
 * It sets the DS1302 reset (RST) and clock (CLK) pins, writes the address, reads the data, and then resets the RST pin.
 *
 * \param[in] addr: Address to read data from
 * \return The data read from the specified address in DS1302
 */
uint8_t
ds1302_read_data(uint8_t addr) {
    uint8_t dat = 0;
    DS1302_RST_LOW; /* Lower the reset pin */
    DS1302_CLK_LOW; /* Lower the clock pin */

    DS1302_RST_HIGH;          /* Raise the reset pin */
    ds1302_write_byte(addr);  /* Write the address */
    dat = ds1302_read_byte(); /* Read the data */

    DS1302_RST_LOW; /* Lower the reset pin to complete the operation */
    return dat;
}

/**
 * \brief Convert BCD (Binary-Coded Decimal) to decimal
 *
 * This function converts BCD values to their decimal equivalent.
 * It takes an array of BCD values and converts each element in-place.
 *
 * \param[in,out] bcd: Array of BCD values to be converted
 * \param[in] times: Number of elements in the array
 */
void
ds1302_bcd_to_dec(uint8_t* bcd, uint8_t times) {
    uint8_t i;

    for (i = 0; i < times; i++) {
        bcd[i] = ((bcd[i] >> 4) * 10) + (bcd[i] & 0x0f);
    }
}

/**
 * \brief Convert decimal to BCD (Binary-Coded Decimal)
 *
 * This function converts decimal values to their BCD equivalent.
 * It takes an array of decimal values and converts each element in-place.
 *
 * \param[in,out] dec: Array of decimal values to be converted
 * \param[in] times: Number of elements in the array
 */
void
ds1302_dec_to_bcd(uint8_t* dec, uint8_t times) {
    uint8_t i;

    for (i = 0; i < times; i++) {
        dec[i] = ((dec[i] / 10) << 4) | (dec[i] % 10);
    }
}

/**
 * \brief Initialize DS1302 module
 *
 * This function initializes the DS1302 module by configuring its pins, converting the current time from decimal to BCD,
 * and writing the BCD values to the corresponding DS1302 registers. It also enables write protection after initialization.
 */
void
ds1302_init(void) {
    ds1302_config(); /* Configure pins */

    ds1302_dec_to_bcd(ds1302_time, 7); /* Convert current time from decimal to BCD */

    ds1302_write_cmd(DS1302_WRITE_PROTECT, 0x80); /* Enable write protection */

    ds1302_write_cmd(DS1302_WRITE_YEAR, ds1302_time[0]);   /* Write year */
    ds1302_write_cmd(DS1302_WRITE_MONTH, ds1302_time[1]);  /* Write month */
    ds1302_write_cmd(DS1302_WRITE_DAY, ds1302_time[2]);    /* Write day */
    ds1302_write_cmd(DS1302_WRITE_HOUR, ds1302_time[3]);   /* Write hour */
    ds1302_write_cmd(DS1302_WRITE_MINUTE, ds1302_time[4]); /* Write minute */
    ds1302_write_cmd(DS1302_WRITE_SECOND, ds1302_time[5]); /* Write second */
    ds1302_write_cmd(DS1302_WRITE_WEEK, ds1302_time[6]);   /* Write week */

    ds1302_write_cmd(DS1302_WRITE_PROTECT, 0x80); /* Enable write protection */
}

/**
 * \brief Read current time from DS1302 module
 *
 * This function reads the current time from the DS1302 module by reading BCD values from the corresponding registers.
 * The BCD values are then converted to decimal.
 */
void
ds1302_read(void) {
    ds1302_time[0] = ds1302_read_data(DS1302_READ_YEAR);   /* Read year */
    ds1302_time[1] = ds1302_read_data(DS1302_READ_MONTH);  /* Read month */
    ds1302_time[2] = ds1302_read_data(DS1302_READ_DAY);    /* Read day */
    ds1302_time[3] = ds1302_read_data(DS1302_READ_HOUR);   /* Read hour */
    ds1302_time[4] = ds1302_read_data(DS1302_READ_MINUTE); /* Read minute */
    ds1302_time[5] = ds1302_read_data(DS1302_READ_SECOND); /* Read second */
    ds1302_time[6] = ds1302_read_data(DS1302_READ_WEEK);   /* Read week */

    ds1302_bcd_to_dec(ds1302_time, 7); /* Convert BCD to decimal */
}