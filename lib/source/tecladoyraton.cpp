/*
 *  Copyright (C) 2010 Alberto Pajuelo Montes <paju1986@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "../headers/tecladoyratonwin.h"
#include <windows.h>

void enviarTecla(int tecla)
{
    switch (tecla)
    {
    case Qt::Key_Return:
        tecla = VK_RETURN;
        break;
    case Qt::Key_Backspace:
        tecla = VK_BACK;
        break;
    case Qt::Key_Escape:
        tecla = VK_ESCAPE;
        break;
    case Qt::Key_Shift:
        tecla = VK_SHIFT;
        break;
    case Qt::Key_CapsLock:
        tecla = VK_CAPITAL;
        break;
    }
    keybd_event(tecla, 0, 0, 0);
    keybd_event(tecla, 0, KEYEVENTF_KEYUP, 0);
}

void hacerClickDerechoP()
{
    mouse_event( MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0 );
}

void hacerClickIzquierdoP()
{
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
}
void hacerClickDerechoS()
{
    mouse_event( MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );
}

void hacerClickIzquierdoS()
{
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}
short comprobarTeclas()
{
    short i = 0;
    for(i = 0; i < 255; i++){
     if(GetAsyncKeyState(i) == -32767)
        return i;
    }
    return 0;
}
