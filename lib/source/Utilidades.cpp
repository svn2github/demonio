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

#include "../headers/Utilidades.h"

Utilidades::Utilidades()
{
    /** Constructor de la clase **/
    this->tamano = 0;
    this->datos.clear();
}


void Utilidades::ventanaEmergente(QString mensaje){
    /** Crea y muestra una ventana emergente con el texto pasado como parámetro **/
    QMessageBox ventana;
    ventana.setText(mensaje);
    ventana.exec();

}
QString Utilidades::leerSocket(QTcpSocket *socket){
    /** Lee datos de un socket **/
    QString datos;
    datos = socket->readAll();
    return datos;
}
void Utilidades::escribirSocket(QString cadena,QTcpSocket *socket){
    /** Escribe datos en un socket **/
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        socket->write(cadena.toLatin1(),cadena.toLatin1().length());
    }
}
QString Utilidades::obtenerRutaAnterior(QString rutaActual){
    /** Obtiene la ruta anterior de una ruta pasada como parámetro **/
    int i;
    QString rutaAnterior;
    rutaActual.replace("\\","/");
    QStringList cachosRuta = rutaActual.split("/");
    rutaAnterior = cachosRuta[0];
    for (i=1;i< cachosRuta.size() - 1;i++){
        rutaAnterior = rutaAnterior + "/" + cachosRuta[i];
    }
    return rutaAnterior;
}
QByteArray Utilidades::leerSocketDatos(QTcpSocket *socket){
    /** Lee datos binarios de un socket **/
    QByteArray datos;
    char *cadena = new char[socket->bytesAvailable() - 1];
    QDataStream entrada(socket);
    entrada.readRawData(cadena,socket->bytesAvailable());
    datos = cadena;
    return datos;
}

void Utilidades::escribirSocketDatos(QByteArray cadena,QTcpSocket *socket){
    /** Escribe datos binarios en un socket **/
    const char *datos = new char[cadena.size() - 1];
    datos = cadena;
    QDataStream salida(socket);
    salida.writeRawData(datos,cadena.size());
}

void Utilidades::enviarArchivo(QString archivo, QTcpSocket *socket){
    /** Envia un archivo por el socket indicado **/
    QByteArray longitud;
    QByteArray datos;
    QFile salida;
    salida.setFileName(archivo);
    salida.open(QIODevice::ReadOnly);
    datos = qCompress(salida.readAll(),9);
    longitud.setNum(datos.size());
    this->escribirSocketDatos(longitud,socket);
    socket->waitForBytesWritten(1000);
    QDataStream enviador(socket);
    enviador.writeRawData(datos,datos.size());
    salida.close();
}

qint64 Utilidades::recibirArchivo(QString rutaArchivo, QTcpSocket *socket)
{
    /** Recibe un archivo enviado con enviarArchivo **/
    if ( tamano == 0 )
    {
        this->datos = socket->readAll();
        tamano = this->datos.toInt();
        if(tamano == 0)
        {
            this->ventanaEmergente("algo no va bien");
        }
        return 0;
    }
    else
    {
        if ( socket->bytesAvailable() == tamano )
        {
            QFile archivo;
            archivo.setFileName(rutaArchivo);
            archivo.open(QIODevice::WriteOnly);
            QDataStream entrada(&archivo);
            this->datos = socket->readAll();
            entrada.writeRawData(qUncompress(datos),datos.size());
            archivo.close();
            this->tamano = 0;
            return 100;
        }
        else
        {
            return (this->tamano / socket->bytesAvailable()) * 100;
        }
    }
}

