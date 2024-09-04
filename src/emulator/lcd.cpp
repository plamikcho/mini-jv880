/*
 * Copyright (C) 2021, 2024 nukeykt
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "lcd_font.h"
#include "mcu.h"

void LCD::LCD_Enable(uint32_t enable)
{
    lcd_enable = enable;
}

void LCD::LCD_Write(uint32_t address, uint8_t data)
{
    if (address == 0)
    {
        if ((data & 0xe0) == 0x20)
        {
            LCD_DL = (data & 0x10) != 0;
            LCD_N = (data & 0x8) != 0;
            LCD_F = (data & 0x4) != 0;
        }
        else if ((data & 0xf8) == 0x8)
        {
            LCD_D = (data & 0x4) != 0;
            LCD_C = (data & 0x2) != 0;
            LCD_B = (data & 0x1) != 0;
        }
        else if ((data & 0xff) == 0x01)
        {
            LCD_DD_RAM = 0;
            LCD_ID = 1;
            memset(LCD_Data, 0x20, sizeof(LCD_Data));
        }
        else if ((data & 0xff) == 0x02)
        {
            LCD_DD_RAM = 0;
        }
        else if ((data & 0xfc) == 0x04)
        {
            LCD_ID = (data & 0x2) != 0;
            LCD_S = (data & 0x1) != 0;
        }
        else if ((data & 0xc0) == 0x40)
        {
            LCD_CG_RAM = (data & 0x3f);
            LCD_RAM_MODE = 0;
        }
        else if ((data & 0x80) == 0x80)
        {
            LCD_DD_RAM = (data & 0x7f);
            LCD_RAM_MODE = 1;
        }
        else
        {
            address += 0;
        }
    }
    else
    {
        if (!LCD_RAM_MODE)
        {
            LCD_CG[LCD_CG_RAM] = data & 0x1f;
            if (LCD_ID)
            {
                LCD_CG_RAM++;
            }
            else
            {
                LCD_CG_RAM--;
            }
            LCD_CG_RAM &= 0x3f;
        }
        else
        {
            if (LCD_N)
            {
                if (LCD_DD_RAM & 0x40)
                {
                    if ((LCD_DD_RAM & 0x3f) < 40)
                        LCD_Data[(LCD_DD_RAM & 0x3f) + 40] = data;
                }
                else
                {
                    if ((LCD_DD_RAM & 0x3f) < 40)
                        LCD_Data[LCD_DD_RAM & 0x3f] = data;
                }
            }
            else
            {
                if (LCD_DD_RAM < 80)
                    LCD_Data[LCD_DD_RAM] = data;
            }
            if (LCD_ID)
            {
                LCD_DD_RAM++;
            }
            else
            {
                LCD_DD_RAM--;
            }
            LCD_DD_RAM &= 0x7f;
        }
    }
    //printf("%i %.2x ", address, data);
    // if (data >= 0x20 && data <= 'z')
    //     printf("%c\n", data);
    //else
    //    printf("\n");
}

void LCD::LCD_FontRenderStandard(int32_t x, int32_t y, uint8_t ch, bool overlay)
{
    uint8_t* f;
    if (ch >= 16)
        f = &lcd_font[ch - 16][0];
    else
        f = &LCD_CG[(ch & 7) * 8];
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            uint32_t col;
            if (f[i] & (1<<(4-j)))
            {
                col = lcd_col1;
            }
            else
            {
                col = lcd_col2;
            }
            int xx = x + i * 6;
            int yy = y + j * 6;
            for (int ii = 0; ii < 5; ii++)
            {
                for (int jj = 0; jj < 5; jj++)
                {
                    if (overlay)
                        lcd_buffer[xx+ii][yy+jj] &= col;
                    else
                        lcd_buffer[xx+ii][yy+jj] = col;
                }
            }
        }
    }
}

uint32_t* LCD::LCD_Update(void)
{
    for (size_t i = 0; i < lcd_height; i++) {
        for (size_t j = 0; j < lcd_width; j++) {
            lcd_buffer[i][j] = lcd_col3;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            uint8_t ch = LCD_Data[i * 40 + j];
            LCD_FontRenderStandard(4 + i * 50, 4 + j * 34, ch);
        }
    }
    
    // cursor
    int j = LCD_DD_RAM % 0x40;
    int i = LCD_DD_RAM / 0x40;
    if (i < 2 && j < 24 && LCD_C)
        LCD_FontRenderStandard(4 + i * 50, 4 + j * 34, '_', true);

    return (uint32_t*)lcd_buffer;
}

void LCD::LCD_SendButton(uint8_t button, int state) {
    uint32_t button_pressed = mcu->mcu_button_pressed;
    int mask = (1 << button);
    if (state) {
        button_pressed |= mask;
    } else {
        button_pressed &= ~mask;
    }
    mcu->mcu_button_pressed = button_pressed;
}

