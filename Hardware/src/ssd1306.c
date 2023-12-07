/*
 * original author:  Tilen Majerle<tilen@majerle.eu>
 * modification for STM32f10x: Alexander Lutsai<s.lyra@ya.ru>

   ----------------------------------------------------------------------
   	Copyright (C) Alexander Lutsai, 2016
    Copyright (C) Tilen Majerle, 2015

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
 */
#include "ssd1306.h"

/**
 * \brief Write command to SSD1306
 *
 * This macro writes a command to the SSD1306 display via I2C communication.
 *
 * \param[in] command: Command to be written
 * \hideinitializer
 */
#define SSD1306_WRITECOMMAND(command) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))

/**
 * \brief Write data to SSD1306
 *
 * This macro writes data to the SSD1306 display via I2C communication.
 *
 * \param[in] data: Data to be written
 * \hideinitializer
 */
#define SSD1306_WRITEDATA(data)       ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))

/**
 * \brief Absolute value macro
 *
 * This macro returns the absolute value of the given expression.
 *
 * \param[in] x: Input expression
 * \hideinitializer
 */
#define ABS(x)                        ((x) > 0 ? (x) : -(x))

/**
 * \brief SSD1306 data buffer
 *
 * This buffer holds the pixel data for the SSD1306 OLED display. It is organized as a one-dimensional array
 * with a size of SSD1306_WIDTH * SSD1306_HEIGHT / 8 bytes, representing the individual pixels on the display.
 */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

/**
 * \brief Private SSD1306 structure
 */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

/**
 * \brief Private variable for SSD1306 structure
 */
static SSD1306_t SSD1306;

/**
 * \brief SSD1306 right horizontal scroll command
 */
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26

/**
 * \brief SSD1306 left horizontal scroll command
 */
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27

/**
 * \brief SSD1306 vertical and right horizontal scroll command
 */
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29

/**
 * \brief SSD1306 vertical and left horizontal scroll command
 */
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A

/**
 * \brief SSD1306 deactivate scroll command
 */
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll

/**
 * \brief SSD1306 activate scroll command
 */
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll

/**
 * \brief SSD1306 set vertical scroll area command
 */
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

/**
 * \brief SSD1306 normal display command
 */
#define SSD1306_NORMALDISPLAY                        0xA6

/**
 * \brief SSD1306 invert display command
 */
#define SSD1306_INVERTDISPLAY                        0xA7

/**
 * \brief Scroll content to the right on the SSD1306 display
 *
 * This function scrolls the content on the SSD1306 display to the right, within the specified row range.
 *
 * \param[in] start_row: Starting row address for scrolling
 * \param[in] end_row: Ending row address for scrolling
 */
void
SSD1306_ScrollRight(uint8_t start_row, uint8_t end_row) {
    SSD1306_WRITECOMMAND(SSD1306_RIGHT_HORIZONTAL_SCROLL); /* Send 0x26 */
    SSD1306_WRITECOMMAND(0x00);                            /* Send dummy */
    SSD1306_WRITECOMMAND(start_row);                       /* Start page address */
    SSD1306_WRITECOMMAND(0X00);                            /* Time interval 5 frames */
    SSD1306_WRITECOMMAND(end_row);                         /* End page address */
    SSD1306_WRITECOMMAND(0X00);
    SSD1306_WRITECOMMAND(0XFF);
    SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); /* Start scroll */
}

/**
 * \brief Scroll content to the left on the SSD1306 display
 *
 * This function scrolls the content on the SSD1306 display to the left, within the specified row range.
 *
 * \param[in] start_row: Starting row address for scrolling
 * \param[in] end_row: Ending row address for scrolling
 */
void
SSD1306_ScrollLeft(uint8_t start_row, uint8_t end_row) {
    SSD1306_WRITECOMMAND(SSD1306_LEFT_HORIZONTAL_SCROLL); /* Send 0x26 */
    SSD1306_WRITECOMMAND(0x00);                           /* Send dummy */
    SSD1306_WRITECOMMAND(start_row);                      /* Start page address */
    SSD1306_WRITECOMMAND(0X00);                           /* Time interval 5 frames */
    SSD1306_WRITECOMMAND(end_row);                        /* End page address */
    SSD1306_WRITECOMMAND(0X00);
    SSD1306_WRITECOMMAND(0XFF);
    SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); /* Start scroll */
}

/**
 * \brief Scroll content diagonally to the right on the SSD1306 display
 *
 * This function scrolls the content diagonally to the right, within the specified row range.
 *
 * \param[in] start_row: Starting row address for scrolling
 * \param[in] end_row: Ending row address for scrolling
 */
void
SSD1306_Scrolldiagright(uint8_t start_row, uint8_t end_row) {
    SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA); /* Set the area */
    SSD1306_WRITECOMMAND(0x00);                             /* Write dummy */
    SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

    SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
    SSD1306_WRITECOMMAND(0x00);
    SSD1306_WRITECOMMAND(start_row);
    SSD1306_WRITECOMMAND(0X00);
    SSD1306_WRITECOMMAND(end_row);
    SSD1306_WRITECOMMAND(0x01);
    SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); /* Start scroll */
}

/**
 * \brief Scroll content diagonally to the left on the SSD1306 display
 *
 * This function scrolls the content diagonally to the left, within the specified row range.
 *
 * \param[in] start_row: Starting row address for scrolling
 * \param[in] end_row: Ending row address for scrolling
 */
void
SSD1306_Scrolldiagleft(uint8_t start_row, uint8_t end_row) {
    SSD1306_WRITECOMMAND(SSD1306_SET_VERTICAL_SCROLL_AREA); /* Set the area */
    SSD1306_WRITECOMMAND(0x00);                             /* Write dummy */
    SSD1306_WRITECOMMAND(SSD1306_HEIGHT);

    SSD1306_WRITECOMMAND(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
    SSD1306_WRITECOMMAND(0x00);
    SSD1306_WRITECOMMAND(start_row);
    SSD1306_WRITECOMMAND(0X00);
    SSD1306_WRITECOMMAND(end_row);
    SSD1306_WRITECOMMAND(0x01);
    SSD1306_WRITECOMMAND(SSD1306_ACTIVATE_SCROLL); /* Start scroll */
}

/**
 * \brief Stop scrolling on the SSD1306 display
 *
 * This function stops the scrolling on the SSD1306 display.
 */
void
SSD1306_Stopscroll(void) {
    SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);
}

/**
 * \brief Invert or restore the display on the SSD1306 display
 *
 * This function inverts or restores the display on the SSD1306 display.
 *
 * \param[in] i: Non-zero value to invert the display, zero to restore the display to normal
 */
void
SSD1306_InvertDisplay(int i) {
    if (i) {
        SSD1306_WRITECOMMAND(SSD1306_INVERTDISPLAY);
    } else {
        SSD1306_WRITECOMMAND(SSD1306_NORMALDISPLAY);
    }
}

/**
 * \brief Draw a bitmap on the SSD1306 display
 *
 * This function draws a bitmap on the SSD1306 display at the specified coordinates with the specified width, height, and color.
 *
 * \param[in] x: X-coordinate of the top-left corner of the bitmap
 * \param[in] y: Y-coordinate of the top-left corner of the bitmap
 * \param[in] bitmap: Pointer to the bitmap data
 * \param[in] w: Width of the bitmap in pixels
 * \param[in] h: Height of the bitmap in pixels
 * \param[in] color: Color of the bitmap pixels (0 for off, non-zero for on)
 */
void
SSD1306_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color) {
    int16_t byteWidth = (w + 7) / 8; /* Bitmap scanline pad = whole byte */
    uint8_t byte = 0;

    for (int16_t j = 0; j < h; j++, y++) {
        for (int16_t i = 0; i < w; i++) {
            if (i & 7) {
                byte <<= 1;
            } else {
                byte = (*(const unsigned char*)(&bitmap[j * byteWidth + i / 8]));
            }
            if (byte & 0x80) {
                SSD1306_DrawPixel(x + i, y, color);
            }
        }
    }
}

/**
 * \brief Initialize SSD1306 OLED display
 *
 * This function initializes the SSD1306 OLED display by performing the following steps:
 * 1. Initialize the I2C communication.
 * 2. Add a delay for stability.
 * 3. Configure the SSD1306 settings, including display mode, addressing mode, and various parameters.
 * 4. Clear the screen and update it.
 * 5. Set default cursor position.
 * 6. Mark the initialization as successful.
 *
 * \return 1 if initialization is successful, 0 otherwise.
 */
uint8_t
SSD1306_Init(void) {
    /* Initialize I2C communication */
    ssd1306_I2C_Init();

    /* A little delay for stability */
    uint32_t delay_counter = 2500;
    while (delay_counter > 0) {
        delay_counter--;
    }

    /* Configure SSD1306 settings */
    SSD1306_WRITECOMMAND(0xAE); /* Display off */
    SSD1306_WRITECOMMAND(0x20); /* Set Memory Addressing Mode */
    SSD1306_WRITECOMMAND(
        0x10); /* 00, Horizontal Addressing Mode; 01, Vertical Addressing Mode; 10, Page Addressing Mode (RESET); 11, Invalid */
    SSD1306_WRITECOMMAND(0xB0); /* Set Page Start Address for Page Addressing Mode, 0-7 */
    SSD1306_WRITECOMMAND(0xC8); /* Set COM Output Scan Direction */
    SSD1306_WRITECOMMAND(0x00); /* Set low column address */
    SSD1306_WRITECOMMAND(0x10); /* Set high column address */
    SSD1306_WRITECOMMAND(0x40); /* Set start line address */
    SSD1306_WRITECOMMAND(0x81); /* Set contrast control register */
    SSD1306_WRITECOMMAND(0xFF);
    SSD1306_WRITECOMMAND(0xA1); /* Set segment re-map 0 to 127 */
    SSD1306_WRITECOMMAND(0xA6); /* Set normal display */
    SSD1306_WRITECOMMAND(0xA8); /* Set multiplex ratio (1 to 64) */
    SSD1306_WRITECOMMAND(0x3F);
    SSD1306_WRITECOMMAND(0xA4); /* 0xA4, Output follows RAM content; 0xA5, Output ignores RAM content */
    SSD1306_WRITECOMMAND(0xD3); /* Set display offset */
    SSD1306_WRITECOMMAND(0x00); /* No offset */
    SSD1306_WRITECOMMAND(0xD5); /* Set display clock divide ratio/oscillator frequency */
    SSD1306_WRITECOMMAND(0xF0); /* Set divide ratio */
    SSD1306_WRITECOMMAND(0xD9); /* Set pre-charge period */
    SSD1306_WRITECOMMAND(0x22);
    SSD1306_WRITECOMMAND(0xDA); /* Set com pins hardware configuration */
    SSD1306_WRITECOMMAND(0x12);
    SSD1306_WRITECOMMAND(0xDB); /* Set vcomh */
    SSD1306_WRITECOMMAND(0x20); /* 0x20, 0.77xVcc */
    SSD1306_WRITECOMMAND(0x8D); /* Set DC-DC enable */
    SSD1306_WRITECOMMAND(0x14);
    SSD1306_WRITECOMMAND(0xAF); /* Turn on SSD1306 panel */

    SSD1306_WRITECOMMAND(SSD1306_DEACTIVATE_SCROLL);

    /* Clear the screen */
    SSD1306_Fill(SSD1306_COLOR_BLACK);

    /* Update the screen */
    SSD1306_UpdateScreen();

    /* Set default cursor position */
    SSD1306.CurrentX = 0;
    SSD1306.CurrentY = 0;

    /* Mark the initialization as successful */
    SSD1306.Initialized = 1;

    /* Return OK */
    return 1;
}

/**
 * \brief Update the content of the SSD1306 OLED screen
 *
 * This function updates the content of the SSD1306 OLED screen by performing the following steps:
 * 1. Set the page address for each of the 8 pages.
 * 2. Set the column address to span the entire width.
 * 3. Write multi-byte data to the display using I2C communication.
 *
 * \note The display buffer is organized in pages, and each page is updated individually.
 */
void
SSD1306_UpdateScreen(void) {
    uint8_t page;

    for (page = 0; page < 8; page++) {
        SSD1306_WRITECOMMAND(0xB0 + page); /* Set page address */
        SSD1306_WRITECOMMAND(0x00);        /* Set low column address */
        SSD1306_WRITECOMMAND(0x10);        /* Set high column address */

        /* Write multi-byte data to the display */
        ssd1306_I2C_WriteMulti(SSD1306_I2C_ADDR, 0x40, &SSD1306_Buffer[SSD1306_WIDTH * page], SSD1306_WIDTH);
    }
}

/**
 * \brief Toggle the inversion mode of the SSD1306 display
 *
 * This function toggles the inversion mode of the SSD1306 display. It updates the `SSD1306.Inverted` flag and
 * performs memory inversion on the display buffer.
 */
void
SSD1306_ToggleInvert(void) {
    /* Toggle invert mode */
    SSD1306.Inverted = !SSD1306.Inverted;

    /* Perform memory inversion */
    for (size_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = ~SSD1306_Buffer[i];
    }
}

/**
 * \brief Fill the SSD1306 display buffer with a specified color
 *
 * This function fills the entire SSD1306 display buffer with the specified color.
 *
 * \param[in] color: Color to fill the buffer with (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_Fill(SSD1306_COLOR_t color) {
    /* Set memory based on the specified color */
    memset(SSD1306_Buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, sizeof(SSD1306_Buffer));
}

/**
 * \brief Draw a pixel on the SSD1306 display
 *
 * This function draws a pixel on the SSD1306 display at the specified coordinates with the specified color.
 *
 * \param[in] x: X-coordinate of the pixel
 * \param[in] y: Y-coordinate of the pixel
 * \param[in] color: Color of the pixel (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawPixel(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        /* Error: Coordinates out of bounds */
        return;
    }

    /* Check if pixels are inverted */
    if (SSD1306.Inverted) {
        color = (SSD1306_COLOR_t)!color;
    }

    /* Set color of the pixel in the display buffer */
    if (color == SSD1306_COLOR_WHITE) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

/**
 * \brief Set the current drawing position on the SSD1306 display
 *
 * This function sets the current drawing position (X and Y coordinates) on the SSD1306 display.
 *
 * \param[in] x: New X-coordinate
 * \param[in] y: New Y-coordinate
 */
void
SSD1306_GotoXY(uint16_t x, uint16_t y) {
    /* Set write pointers */
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

/**
 * \brief Write a character to the SSD1306 display using a specified font
 *
 * This function writes a character to the SSD1306 display at the current position using the specified font.
 *
 * \param[in] ch: Character to write
 * \param[in] Font: Pointer to the font definition
 * \param[in] color: Color of the character (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 * \return Written character
 */
char
SSD1306_Putc(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
    uint32_t i, b, j;

    /* Check available space in LCD */
    if (SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth)
        || SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)) {
        /* Error: Insufficient space */
        return 0;
    }

    /* Go through font */
    for (i = 0; i < Font->FontHeight; i++) {
        b = Font->data[(ch - 32) * Font->FontHeight + i];
        for (j = 0; j < Font->FontWidth; j++) {
            if ((b << j) & 0x8000) {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)color);
            } else {
                SSD1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
            }
        }
    }

    /* Increase pointer */
    SSD1306.CurrentX += Font->FontWidth;

    /* Return character written */
    return ch;
}

/**
 * \brief Write a string to the SSD1306 display using a specified font
 *
 * This function writes a string to the SSD1306 display at the current position using the specified font.
 *
 * \param[in] str: Null-terminated string to write
 * \param[in] Font: Pointer to the font definition
 * \param[in] color: Color of the characters (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 * \return First character that couldn't be written (0 if successful)
 */
char
SSD1306_Puts(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
    /* Write characters */
    while (*str) {
        /* Write character by character */
        if (SSD1306_Putc(*str, Font, color) != *str) {
            /* Return the first character that couldn't be written */
            return *str;
        }

        /* Increase string pointer */
        str++;
    }

    /* Everything OK, zero should be returned */
    return *str;
}

/**
 * \brief Draw a line on the SSD1306 display
 *
 * This function draws a line on the SSD1306 display between two specified points with a specified color.
 *
 * \param[in] x0: X coordinate of the starting point
 * \param[in] y0: Y coordinate of the starting point
 * \param[in] x1: X coordinate of the ending point
 * \param[in] y1: Y coordinate of the ending point
 * \param[in] c: Color of the line (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c) {
    int16_t dx, dy, sx, sy, err, e2, i, tmp;

    /* Check for overflow and adjust coordinates if necessary */
    if (x0 >= SSD1306_WIDTH) {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH) {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT) {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT) {
        y1 = SSD1306_HEIGHT - 1;
    }

    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = ((dx > dy) ? dx : -dy) / 2;

    if (dx == 0) {
        if (y1 < y0) {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0) {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Vertical line */
        for (i = y0; i <= y1; i++) {
            SSD1306_DrawPixel(x0, i, c);
        }

        /* Return from function */
        return;
    }

    if (dy == 0) {
        if (y1 < y0) {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        if (x1 < x0) {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        /* Horizontal line */
        for (i = x0; i <= x1; i++) {
            SSD1306_DrawPixel(i, y0, c);
        }

        /* Return from function */
        return;
    }

    while (1) {
        SSD1306_DrawPixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * \brief Draw a rectangle on the SSD1306 display
 *
 * This function draws a rectangle on the SSD1306 display with the specified position, width, height, and color.
 *
 * \param[in] x: X coordinate of the top-left corner of the rectangle
 * \param[in] y: Y coordinate of the top-left corner of the rectangle
 * \param[in] w: Width of the rectangle
 * \param[in] h: Height of the rectangle
 * \param[in] c: Color of the rectangle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
    /* Check input parameters */
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH) {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT) {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw 4 lines to form a rectangle */
    SSD1306_DrawLine(x, y, x + w, y, c);         /* Top line */
    SSD1306_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
    SSD1306_DrawLine(x, y, x, y + h, c);         /* Left line */
    SSD1306_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

/**
 * \brief Draw a filled rectangle on the SSD1306 display
 *
 * This function draws a filled rectangle on the SSD1306 display with the specified position, width, height, and color.
 *
 * \param[in] x: X coordinate of the top-left corner of the rectangle
 * \param[in] y: Y coordinate of the top-left corner of the rectangle
 * \param[in] w: Width of the rectangle
 * \param[in] h: Height of the rectangle
 * \param[in] c: Color of the rectangle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c) {
    uint8_t i;

    /* Check input parameters */
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        /* Return error */
        return;
    }

    /* Check width and height */
    if ((x + w) >= SSD1306_WIDTH) {
        w = SSD1306_WIDTH - x;
    }
    if ((y + h) >= SSD1306_HEIGHT) {
        h = SSD1306_HEIGHT - y;
    }

    /* Draw filled lines to form a filled rectangle */
    for (i = 0; i <= h; i++) {
        SSD1306_DrawLine(x, y + i, x + w, y + i, c);
    }
}

/**
 * \brief Draw a triangle on the SSD1306 display
 *
 * This function draws a triangle on the SSD1306 display with the specified vertices and color.
 *
 * \param[in] x1: X coordinate of the first vertex
 * \param[in] y1: Y coordinate of the first vertex
 * \param[in] x2: X coordinate of the second vertex
 * \param[in] y2: Y coordinate of the second vertex
 * \param[in] x3: X coordinate of the third vertex
 * \param[in] y3: Y coordinate of the third vertex
 * \param[in] color: Color of the triangle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3,
                     SSD1306_COLOR_t color) {
    /* Draw lines connecting the vertices to form a triangle */
    SSD1306_DrawLine(x1, y1, x2, y2, color);
    SSD1306_DrawLine(x2, y2, x3, y3, color);
    SSD1306_DrawLine(x3, y3, x1, y1, color);
}

/**
 * \brief Draw a filled triangle on the SSD1306 display
 *
 * This function draws a filled triangle on the SSD1306 display with the specified vertices and color.
 *
 * \param[in] x1: X coordinate of the first vertex
 * \param[in] y1: Y coordinate of the first vertex
 * \param[in] x2: X coordinate of the second vertex
 * \param[in] y2: Y coordinate of the second vertex
 * \param[in] x3: X coordinate of the third vertex
 * \param[in] y3: Y coordinate of the third vertex
 * \param[in] color: Color of the filled triangle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3,
                           SSD1306_COLOR_t color) {
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, yinc1 = 0, yinc2 = 0, den = 0, num = 0,
            numadd = 0, numpixels = 0, curpixel = 0;

    /* Calculate the parameters for drawing a filled triangle using the Bresenham's line algorithm */
    deltax = ABS(x2 - x1);
    deltay = ABS(y2 - y1);
    x = x1;
    y = y1;

    if (x2 >= x1) {
        xinc1 = 1;
        xinc2 = 1;
    } else {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1) {
        yinc1 = 1;
        yinc2 = 1;
    } else {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay) {
        xinc1 = 0;
        yinc2 = 0;
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;
    } else {
        xinc2 = 0;
        yinc1 = 0;
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;
    }

    /* Draw each horizontal line of the filled triangle */
    for (curpixel = 0; curpixel <= numpixels; curpixel++) {
        SSD1306_DrawLine(x, y, x3, y3, color);

        num += numadd;
        if (num >= den) {
            num -= den;
            x += xinc1;
            y += yinc1;
        }
        x += xinc2;
        y += yinc2;
    }
}

/**
 * \brief Draw a circle on the SSD1306 display
 *
 * This function draws a circle on the SSD1306 display with the specified center, radius, and color.
 *
 * \param[in] x0: X coordinate of the center
 * \param[in] y0: Y coordinate of the center
 * \param[in] r: Radius of the circle
 * \param[in] c: Color of the circle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    /* Draw the four cardinal points and calculate the next ones using the Bresenham's circle algorithm */
    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawPixel(x0 + x, y0 + y, c);
        SSD1306_DrawPixel(x0 - x, y0 + y, c);
        SSD1306_DrawPixel(x0 + x, y0 - y, c);
        SSD1306_DrawPixel(x0 - x, y0 - y, c);

        SSD1306_DrawPixel(x0 + y, y0 + x, c);
        SSD1306_DrawPixel(x0 - y, y0 + x, c);
        SSD1306_DrawPixel(x0 + y, y0 - x, c);
        SSD1306_DrawPixel(x0 - y, y0 - x, c);
    }
}

/**
 * \brief Draw a filled circle on the SSD1306 display
 *
 * This function draws a filled circle on the SSD1306 display with the specified center, radius, and color.
 *
 * \param[in] x0: X coordinate of the center
 * \param[in] y0: Y coordinate of the center
 * \param[in] r: Radius of the circle
 * \param[in] c: Color of the filled circle (SSD1306_COLOR_BLACK or SSD1306_COLOR_WHITE)
 */
void
SSD1306_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    /* Draw the central horizontal line and calculate the next ones using the Bresenham's circle algorithm */
    SSD1306_DrawPixel(x0, y0 + r, c);
    SSD1306_DrawPixel(x0, y0 - r, c);
    SSD1306_DrawPixel(x0 + r, y0, c);
    SSD1306_DrawPixel(x0 - r, y0, c);
    SSD1306_DrawLine(x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        SSD1306_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
        SSD1306_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

        SSD1306_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
        SSD1306_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}

/**
 * \brief Clear the SSD1306 display
 *
 * This function clears the entire SSD1306 display by filling it with black pixels.
 * After clearing, the screen update function should be called to apply changes.
 */
void
SSD1306_Clear(void) {
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    SSD1306_UpdateScreen();
}

/**
 * \brief Turn ON the SSD1306 display
 *
 * This function sends the necessary commands to turn ON the SSD1306 display.
 * It should be followed by a screen update function to apply changes.
 */
void
SSD1306_ON(void) {
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x14);
    SSD1306_WRITECOMMAND(0xAF);
}

/**
 * \brief Turn OFF the SSD1306 display
 *
 * This function sends the necessary commands to turn OFF the SSD1306 display.
 * It should be followed by a screen update function to apply changes.
 */
void
SSD1306_OFF(void) {
    SSD1306_WRITECOMMAND(0x8D);
    SSD1306_WRITECOMMAND(0x10);
    SSD1306_WRITECOMMAND(0xAE);
}

/* I2C here */

/**
 * \brief Write the SCL (Clock) signal for the SSD1306
 *
 * This macro sets or clears the SCL (Clock) signal for the SSD1306 by controlling the corresponding GPIO pin.
 *
 * \param x The value to be written to the SCL pin (1 for high, 0 for low).
 * \hideinitializer
 */
#define SSD1306_W_SCL(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))

/**
 * \brief Write the SDA (Data) signal for the SSD1306
 *
 * This macro sets or clears the SDA (Data) signal for the SSD1306 by controlling the corresponding GPIO pin.
 *
 * \param x The value to be written to the SDA pin (1 for high, 0 for low).
 * \hideinitializer
 */
#define SSD1306_W_SDA(x) GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/**
 * \brief Initialize the I2C communication for the SSD1306
 *
 * This function initializes the GPIO pins used for I2C communication with the SSD1306.
 */
void
ssd1306_I2C_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
     * USART1 GPIO Configuration
     *
     * PB8   ------> SCL
     * PB9   ------> SDA
     */

    /* Configure SCL pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure SDA pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Set SCL and SDA to high */
    SSD1306_W_SCL(1);
    SSD1306_W_SDA(1);
}

/**
 * \brief Start the I2C communication for the SSD1306
 *
 * This function generates the start condition for I2C communication with the SSD1306.
 */
void ssd1306_I2C_Start(void) {
    SSD1306_W_SDA(1);
    SSD1306_W_SCL(1);
    SSD1306_W_SDA(0);
    SSD1306_W_SCL(0);
}

/**
 * \brief Stop the I2C communication for the SSD1306
 *
 * This function generates the stop condition for I2C communication with the SSD1306.
 */
void ssd1306_I2C_Stop(void) {
    SSD1306_W_SDA(0);
    SSD1306_W_SCL(1);
    SSD1306_W_SDA(1);
}

/**
 * \brief Send a byte over I2C to the SSD1306
 *
 * This function sends a byte over I2C to the SSD1306. It sends the MSB first.
 *
 * \param Byte: Byte to be sent
 */
void ssd1306_I2C_SendByte(uint8_t Byte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        SSD1306_W_SDA(Byte & (0x80 >> i));
        SSD1306_W_SCL(1);
        SSD1306_W_SCL(0);
    }
    SSD1306_W_SCL(1); // An extra clock to handle the acknowledgment signal, not processing the acknowledgment
    SSD1306_W_SCL(0);
}


/**
 * \brief Write multiple bytes to the SSD1306 via I2C
 *
 * This function starts by sending the I2C start condition, followed by the device address with the
 * write flag, then the register address, and finally the data bytes. It concludes with the I2C stop
 * condition.
 *
 * \param address:   I2C address of the device
 * \param reg:       Register address to write to
 * \param data:      Pointer to the data buffer
 * \param count:     Number of bytes to write
 */
void ssd1306_I2C_WriteMulti(uint8_t address, uint8_t reg, uint8_t* data, uint16_t count) {
    ssd1306_I2C_Start();
    ssd1306_I2C_SendByte(address); // Slave address
    ssd1306_I2C_SendByte(reg);     // Register address
    for (uint16_t i = 0; i < count; ++i) {
        ssd1306_I2C_SendByte(data[i]);
    }
    ssd1306_I2C_Stop();
}

/**
 * \brief Write a single byte to the SSD1306 via I2C
 *
 * This function starts by sending the I2C start condition, followed by the device address with the
 * write flag, then the register address, and finally the data byte. It concludes with the I2C stop
 * condition.
 *
 * \param address:   I2C address of the device
 * \param reg:       Register address to write to
 * \param data:      Data byte to write
 */
void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data) {
    ssd1306_I2C_Start();
    ssd1306_I2C_SendByte(address); // Slave address
    ssd1306_I2C_SendByte(reg);     // Register address
    ssd1306_I2C_SendByte(data);
    ssd1306_I2C_Stop();
}
