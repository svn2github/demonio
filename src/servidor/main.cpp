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

#include <QtGui/QApplication>
#include <qxmpp/QXmppLogger.h>
#include "mainwindow.h"
#ifndef Q_WS_X11
#include <QtPlugin>
Q_IMPORT_PLUGIN(qjpeg);
#endif

#ifdef _MSC_VER
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    int argc = 0;
    char *argv[1];
#else
int main(int argc, char *argv[])
{
#endif
    QApplication a(argc, argv);
    MainWindow w;
    QXmppLogger::getLogger()->setLoggingType(QXmppLogger::StdoutLogging);
    //w.show();
    return a.exec();
}
