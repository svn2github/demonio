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

#include "../headers/webcamwin.h"
#include <QFile>
HWND hwndVideo; // video capture window
void encender()
{
    ::hwndVideo = capCreateCaptureWindowA("captura",WS_POPUP ,1,1 ,240 ,320,NULL,0);
    capDriverConnect(::hwndVideo,0);
    capPreviewRate(::hwndVideo,100);
    capPreviewScale(::hwndVideo,false);
}

void apagar()
{
    capDriverDisconnect(::hwndVideo);
    DestroyWindow(::hwndVideo);
    ::hwndVideo = NULL;
}

QPixmap capturar()
{
    capGrabFrame(::hwndVideo);
    QPixmap imagen;
    QFile crear;
    crear.setFileName("data");
    crear.open(QFile::WriteOnly);
    crear.close();
    capFileSaveDIB(::hwndVideo,L"data");
    imagen.load("data");
    return imagen;
}
