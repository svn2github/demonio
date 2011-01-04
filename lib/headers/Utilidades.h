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

#ifndef UTILIDADES_H
#define UTILIDADES_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

class Utilidades {
    public:
        Utilidades();
        void ventanaEmergente(QString mensaje);
        QString obtenerRutaAnterior(QString rutaActual);
        QString leerSocket(QTcpSocket *socket);
        QByteArray leerSocketDatos(QTcpSocket *socket);
        QByteArray datos;
        qint64 tamano;
        void escribirSocket(QString cadena,QTcpSocket *socket);
        void escribirSocketDatos(QByteArray cadena,QTcpSocket *socket);
        void enviarArchivo(QString archivo,QTcpSocket *socket);
        qint64 recibirArchivo(QString rutaArchivo,QTcpSocket *socket);
    private:

    };
#endif // UTILIDADES_H


