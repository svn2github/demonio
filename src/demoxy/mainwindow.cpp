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
    QTcpSocket *socketPrincipal = new QTcpSocket(this);
    QTcpSocket *socketEscritorio = new QTcpSocket(this);
    QTcpSocket *socketWebcam = new QTcpSocket(this);
    QTcpSocket *socketArchivos = new QTcpSocket(this);

    QTcpSocket *socketPrincipalCliente = new QTcpSocket(this);
    QTcpSocket *socketEscritorioCliente = new QTcpSocket(this);
    QTcpSocket *socketWebcamCliente = new QTcpSocket(this);
    QTcpSocket *socketArchivosCliente = new QTcpSocket(this);

    ui->setupUi(this);
    servidorPrincipal.listen( QHostAddress::Any,1234);
    servidorArchivos.listen ( QHostAddress::Any,2345);
    servidorEscritorio.listen ( QHostAddress::Any,3456);
    servidorWebcam.listen ( QHostAddress::Any,4567);

    servidorPrincipalCliente.listen( QHostAddress::Any,1111);
    servidorArchivosCliente.listen ( QHostAddress::Any,2222);
    servidorEscritorioCliente.listen ( QHostAddress::Any,3333);
    servidorWebcamCliente.listen ( QHostAddress::Any,4444);

    connect ( &servidorPrincipal,SIGNAL ( newConnection() ),this,SLOT ( conectadoPrincipal() ) );
    connect ( &servidorArchivos,SIGNAL ( newConnection() ),this,SLOT ( conectadoArchivos() ) );
    connect ( &servidorEscritorio,SIGNAL ( newConnection() ),this,SLOT ( conectadoEscritorio() ) );
    connect ( &servidorWebcam,SIGNAL ( newConnection() ),this,SLOT ( conectadoWebcam() ) );

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
    socketPrincipal = servidorPrincipal.nextPendingConnection();
    socketPrincipalCliente->write("conectado|@|");
    connect ( socketPrincipal,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipal() ) );

}
void MainWindow::conectadoEscritorio()
{
    socketEscritorio = servidorEscritorio.nextPendingConnection();
    connect ( socketEscritorio,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorio() ) );
}
void MainWindow::conectadoWebcam()
{
    socketWebcam = servidorWebcam.nextPendingConnection();
    connect ( socketWebcam,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcam() ) );
}
void MainWindow::conectadoArchivos()
{
    socketArchivos = servidorArchivos.nextPendingConnection();
    connect ( socketArchivos,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivos() ) );
}
void MainWindow::conectadoPrincipalCliente()
{
    socketPrincipalCliente = servidorPrincipalCliente.nextPendingConnection();
    connect ( socketPrincipalCliente,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipalCliente() ) );
}
void MainWindow::conectadoEscritorioCliente()
{
    socketEscritorioCliente = servidorEscritorioCliente.nextPendingConnection();
    connect ( socketEscritorioCliente,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorioCliente() ) );
}
void MainWindow::conectadoWebcamCliente()
{
    socketWebcamCliente = servidorWebcamCliente.nextPendingConnection();
    connect ( socketWebcamCliente,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcamCliente() ) );
}
void MainWindow::conectadoArchivosCliente()
{
    socketArchivosCliente = servidorArchivosCliente.nextPendingConnection();
    connect ( socketArchivosCliente,SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivosCliente() ) );
}


void MainWindow::llegadaDatosPrincipal()
{
    socketPrincipalCliente->write(socketPrincipal->readAll());
}
void MainWindow::llegadaDatosEscritorio()
{
    socketEscritorioCliente->write(socketEscritorio->readAll());
}
void MainWindow::llegadaDatosWebcam()
{
    socketWebcamCliente->write(socketWebcam->readAll());
}
void MainWindow::llegadaDatosArchivos()
{
    socketArchivosCliente->write(socketArchivos->readAll());
}

void MainWindow::llegadaDatosPrincipalCliente()
{
    socketPrincipal->write(socketPrincipalCliente->readAll());
}
void MainWindow::llegadaDatosEscritorioCliente()
{
    socketEscritorio->write(socketEscritorioCliente->readAll());
}
void MainWindow::llegadaDatosWebcamCliente()
{
    socketWebcam->write(socketWebcamCliente->readAll());
}
void MainWindow::llegadaDatosArchivosCliente()
{
    socketArchivos->write(socketArchivosCliente->readAll());
}
