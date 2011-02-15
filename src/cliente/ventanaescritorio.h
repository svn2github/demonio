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

#ifndef VENTANAESCRITORIO_H
#define VENTANAESCRITORIO_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFileDialog>
#include "../../lib/headers/Utilidades.h"
#include "imageescritorio.h"
#include <QTimer>
#include <QThread>

namespace Ui
  {
  class ventanaEscritorio;
}

class reconstruccion: public QObject
{
    Q_OBJECT
public:
    reconstruccion();
    QImage *imagen1;
    int tamano;
    QByteArray datos;
signals:
    void imagen(QImage);
    void error();
public slots:
    void procesarImagen(QByteArray);
    void llegadaDatos(QTcpSocket *socket);
};
class eventos: public QObject
{
    Q_OBJECT
public:
    void pulsado(QMouseEvent *boton);
    void soltado(QMouseEvent *boton);
    void tecla(QKeyEvent *teclado);
};
class ventanaEscritorio : public QMainWindow
  {
    Q_OBJECT
  public:
    reconstruccion reco;
    ventanaEscritorio ( QWidget *parent = 0 );
    ~ventanaEscritorio();
    QTcpSocket *socketEscritorio[100];
    QLabel *img;
    QLabel *imageEscritorio;
    QTimer refresco;
    QPixmap *captura1;
    QThread hilo;
    QString teclas;
    int arrastrax;
    int arrastray;
    bool interruptor;
    int cont;
    unsigned long int numCapturas;
    Utilidades util;
    int conexiones;
    int activo;
    int alto;
    int ancho;
  signals:
    void pulsado();
    void soltado();
    void click(QString cadena);
    void tecla(QString cadena);
    void recibir(QTcpSocket *);
  public slots:
    void llegadaDatos();
    void botonCapturar();
    void cambioCalidad();
    void ponerCaptura (QImage captura1);
    void guardarCaptura ( QString rutaArchivo,QPixmap captura );
    void botonGuardar();
    void checkStreaming();
    void ponerTiempo();
    void maximizar();
    void ocultar();
  protected:
    void changeEvent ( QEvent *e );
    void keyReleaseEvent(QKeyEvent *teclado);
    void mouseReleaseEvent(QMouseEvent *boton);
    void mousePressEvent(QMouseEvent *boton);
  private:
    Ui::ventanaEscritorio *ui;
  };

#endif // VENTANAESCRITORIO_H
