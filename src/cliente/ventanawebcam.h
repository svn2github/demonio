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

#ifndef VENTANAWEBCAM_H
#define VENTANAWEBCAM_H

#include <QMainWindow>
#include <QTcpSocket>
#include "../../lib/headers/Utilidades.h"

namespace Ui {
    class ventanawebcam;
}

class ventanawebcam : public QMainWindow
{
    Q_OBJECT

public:
    explicit ventanawebcam(QWidget *parent = 0);
    ~ventanawebcam();
     QTcpSocket *socketWebcam[100];
     Utilidades util;
     int conexiones;
     int activo;
     int calidad;
     bool activar;
     unsigned long int numCapturas;
 public slots:
     void llegadaDatos();
     void capturar();
     void cambioCalidad();
     void encender();
     void apagar();
     void guardarCaptura();

private:
    Ui::ventanawebcam *ui;
};

#endif // VENTANAWEBCAM_H
