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

#include "../headers/tecladoyratonlin.h"
#include <QMessageBox>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

void mouseClick(int button,bool presionado)
{
        //Código extraido de http://snippets.dzone.com/tag/x11
        Display *display = XOpenDisplay(NULL);

        XEvent event;

        if(display == NULL)
        {
                fprintf(stderr, "Errore nell'apertura del Display !!!\n");
                exit(EXIT_FAILURE);
        }

        memset(&event, 0x00, sizeof(event));

        event.type = ButtonPress;
        event.xbutton.button = button;
        event.xbutton.same_screen = True;

        XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

        event.xbutton.subwindow = event.xbutton.window;

        while(event.xbutton.subwindow)
        {
                event.xbutton.window = event.xbutton.subwindow;

                XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
        }
        if(presionado)
        {
            if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Errore nell'invio dell'evento !!!\n");
        }
        XFlush(display);

        usleep(100000);

        event.type = ButtonRelease;
        event.xbutton.state = 0x100;
        if(!presionado)
        {
            if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Errore nell'invio dell'evento !!!\n");
        }
        XFlush(display);

        XCloseDisplay(display);
}

void enviarTecla(int tecla)
{

    Display *display = XOpenDisplay(NULL);
    XKeyEvent event;

    switch (tecla)
    {
    case Qt::Key_Return:
        tecla = XK_Return ;
        break;
    case Qt::Key_Backspace:
        tecla = XK_BackSpace ;
        break;
    case Qt::Key_Escape:
        tecla = XK_Escape;
        break;
    case Qt::Key_Shift:
        tecla = XK_Shift_L ;
        break;
    case Qt::Key_CapsLock:
        tecla = XK_Caps_Lock;
        break;
    }
      int a;
      event.display = display;
      XGetInputFocus(display,&event.window,&a);
      event.root = 0;
      event.subwindow = None;
      event.time = CurrentTime;
      event.x = 1;
      event.y = 1;
      event.x_root = 1;
      event.y_root = 1;
      event.same_screen = TRUE;

      event.type = KeyPress;  //event.type = KeyRelease;
      event.keycode = XKeysymToKeycode (display,tecla);
      event.state = 0;

      XSendEvent(event.display, event.window, TRUE, KeyPressMask, (XEvent *)&event);


      event.display = display;
      XGetInputFocus(display,&event.window,&a);
      event.root = 0;
      event.subwindow = None;
      event.time = CurrentTime;
      event.x = 1;
      event.y = 1;
      event.x_root = 1;
      event.y_root = 1;
      event.same_screen = TRUE;

      event.type = KeyRelease;
      event.keycode = XKeysymToKeycode (display,tecla);
      event.state = 0;

      XSendEvent(event.display, event.window, TRUE, KeyReleaseMask, (XEvent *)&event);
      XCloseDisplay(display);
}
void hacerClickDerechoP()
{
    mouseClick(3,true);
}
void hacerClickIzquierdoP()
{
    mouseClick(1,true);
}
void hacerClickDerechoS()
{
    mouseClick(3,false);
}
void hacerClickIzquierdoS()
{
    mouseClick(1,false);
}

int fooling(char c) {
  switch(c) {
    case 1: return 0;
    case 2: return 1;
    case 4: return 2;
    case 8: return 3;
    case 16: return 4;
    case 32: return 5;
    case 64: return 6;
    case 128: return 7;
  }
}

char nk2[32];

short comprobarTeclas()
{
    char nk[32];
    memset(nk,0,sizeof(char));
    int i =0;
    unsigned int keycode;
    Display *display;
    display = XOpenDisplay(NULL);
    fflush(stdout);
    XQueryKeymap(display, nk);
    keycode = 0;
    while(keycode == 0 && i<32)
    {
        if(nk[i]!=0 && nk[i] != nk2[i])
            keycode=i*8+fooling(nk[i]);
        i++;
    }
    char ks = XKeycodeToKeysym(display, keycode, 0);
    XCloseDisplay(display);
    for(i=0;i<32;i++)
    {
        nk2[i] = nk[i];
    }
    return ks;
}
