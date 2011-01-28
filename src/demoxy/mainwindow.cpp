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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    conexiones1 = 0;
    conexiones2 = 0;
    activo = 0;
    ui->setupUi(this);
    servidorPrincipal.listen( QHostAddress::Any,1234);
    servidorArchivos.listen ( QHostAddress::Any,2345);
    servidorEscritorio.listen ( QHostAddress::Any,3456);
    servidorWebcam.listen ( QHostAddress::Any,4567);
    servidorDemoxy.listen ( QHostAddress::Any,5555);

    servidorPrincipalCliente.listen( QHostAddress::Any,1111);
    servidorArchivosCliente.listen ( QHostAddress::Any,2222);
    servidorEscritorioCliente.listen ( QHostAddress::Any,3333);
    servidorWebcamCliente.listen ( QHostAddress::Any,4444);

    connect ( &servidorPrincipal,SIGNAL ( newConnection() ),this,SLOT ( conectadoPrincipal() ) );
    connect ( &servidorArchivos,SIGNAL ( newConnection() ),this,SLOT ( conectadoArchivos() ) );
    connect ( &servidorEscritorio,SIGNAL ( newConnection() ),this,SLOT ( conectadoEscritorio() ) );
    connect ( &servidorWebcam,SIGNAL ( newConnection() ),this,SLOT ( conectadoWebcam() ) );
    connect ( &servidorDemoxy,SIGNAL(newConnection()),this,SLOT(conectadoDemoxy()));

    connect ( &servidorPrincipalCliente,SIGNAL ( newConnection() ),this,SLOT ( conectadoPrincipalCliente() ) );
    connect ( &servidorArchivosCliente,SIGNAL ( newConnection() ),this,SLOT ( conectadoArchivosCliente() ) );
    connect ( &servidorEscritorioCliente,SIGNAL ( newConnection() ),this,SLOT ( conectadoEscritorioCliente() ) );
    connect ( &servidorWebcamCliente,SIGNAL ( newConnection() ),this,SLOT ( conectadoWebcamCliente() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::conectadoPrincipal()
{
    activo = 1;
    conexiones1++;
    socketPrincipal[conexiones1] = new QTcpSocket(this);
    socketPrincipal[conexiones1] = servidorPrincipal.nextPendingConnection();
    this->socketDemoxy->write("conectado|@|");
    connect ( socketPrincipal[conexiones1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipal() ) );

}
void MainWindow::conectadoEscritorio()
{
    socketEscritorio[conexiones1] = new QTcpSocket(this);
    socketEscritorio[conexiones1] = servidorEscritorio.nextPendingConnection();
    connect ( socketEscritorio[conexiones1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorio() ) );
}
void MainWindow::conectadoWebcam()
{
    socketWebcam[conexiones1] = new QTcpSocket(this);
    socketWebcam[conexiones1] = servidorWebcam.nextPendingConnection();
    connect ( socketWebcam[conexiones1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcam() ) );
}
void MainWindow::conectadoArchivos()
{
    socketArchivos[conexiones1] = new QTcpSocket(this);
    socketArchivos[conexiones1] = servidorArchivos.nextPendingConnection();
    connect ( socketArchivos[conexiones1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivos() ) );
}
void MainWindow::conectadoDemoxy()
{
        socketDemoxy = servidorDemoxy.nextPendingConnection();
        connect (socketDemoxy,SIGNAL(readyRead()),this,SLOT(llegadaDatosDemoxy()));        
}
void MainWindow::conectadoPrincipalCliente()
{
    conexiones2++;
    socketPrincipalCliente[conexiones2] = new QTcpSocket(this);
    socketPrincipalCliente[conexiones2] = servidorPrincipalCliente.nextPendingConnection();
    connect ( socketPrincipalCliente[conexiones2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipalCliente() ) );
}
void MainWindow::conectadoEscritorioCliente()
{
    socketEscritorioCliente[conexiones2] = new QTcpSocket(this);
    socketEscritorioCliente[conexiones2] = servidorEscritorioCliente.nextPendingConnection();
    connect ( socketEscritorioCliente[conexiones2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorioCliente() ) );
}
void MainWindow::conectadoWebcamCliente()
{
    socketWebcamCliente[conexiones2] = new QTcpSocket(this);
    socketWebcamCliente[conexiones2] = servidorWebcamCliente.nextPendingConnection();
    connect ( socketWebcamCliente[conexiones2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcamCliente() ) );
}
void MainWindow::conectadoArchivosCliente()
{
    socketArchivosCliente[conexiones2] = new QTcpSocket(this);
    socketArchivosCliente[conexiones2] = servidorArchivosCliente.nextPendingConnection();
    connect ( socketArchivosCliente[conexiones2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivosCliente() ) );
}


void MainWindow::llegadaDatosPrincipal()
{
    socketPrincipalCliente[activo]->write(socketPrincipal[activo]->readAll());
}
void MainWindow::llegadaDatosEscritorio()
{
    socketEscritorioCliente[activo]->write(socketEscritorio[activo]->readAll());
}
void MainWindow::llegadaDatosWebcam()
{
    socketWebcamCliente[activo]->write(socketWebcam[activo]->readAll());
}
void MainWindow::llegadaDatosArchivos()
{
    socketArchivosCliente[activo]->write(socketArchivos[activo]->readAll());
}

void MainWindow::llegadaDatosPrincipalCliente()
{
    socketPrincipal[activo]->write(socketPrincipalCliente[activo]->readAll());
}
void MainWindow::llegadaDatosEscritorioCliente()
{
    socketEscritorio[activo]->write(socketEscritorioCliente[activo]->readAll());
}
void MainWindow::llegadaDatosWebcamCliente()
{
    socketWebcam[activo]->write(socketWebcamCliente[activo]->readAll());
}
void MainWindow::llegadaDatosArchivosCliente()
{
    socketArchivos[activo]->write(socketArchivosCliente[activo]->readAll());
}
void MainWindow::llegadaDatosDemoxy()
{

}
