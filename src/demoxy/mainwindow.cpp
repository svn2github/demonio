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
    conexionesArchivos1 = 0;
    conexionesEscritorio1 = 0;
    conexionesWebcam1 = 0;
    conexiones2 = 0;
    conexionesArchivos2 = 0;
    conexionesEscritorio2 = 0;
    conexionesWebcam2 = 0;
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

    socketDemoxy = new QTcpSocket(this);

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
    socketPrincipal[conexiones1] = new QTcpSocket(this);
    socketPrincipal[conexiones1] = servidorPrincipal.nextPendingConnection();
    if (this->socketDemoxy->state() == QAbstractSocket::ConnectedState)
    {
        this->socketDemoxy->write("conectado|@|");
    }
    connect ( socketPrincipal[conexiones1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipal() ) );
    connect ( socketPrincipal[conexiones1],SIGNAL(disconnected()),this,SLOT(desconectadoPrincipal()));
    conexiones1++;

}
void MainWindow::desconectadoPrincipal()
{
    int j,indice = 0;
    for(j=0;j<conexiones1;j++)
    {
        if(socketPrincipal[j]->state() != QAbstractSocket::ConnectedState)
        {
            indice = j;
        }
    }
    socketPrincipal[indice]->close();
    socketArchivos[indice]->close();
    socketEscritorio[indice]->close();
    socketWebcam[indice]->close();
    socketPrincipalCliente[indice]->close();
    socketArchivosCliente[indice]->close();
    socketEscritorioCliente[indice]->close();
    socketWebcamCliente[indice]->close();

    //socket[indice]->~QTcpSocket(); //Da fallo de segmentaciÃ³n al intentar liberar la memoria del socket
    for ( j=indice;conexiones1 - 1 > j;j++ )
      {
        socketPrincipal[j] = socketPrincipal[j + 1];
        socketArchivos[j] = socketArchivos[j + 1];
        socketEscritorio[j] = socketEscritorio[j + 1];
        socketWebcam[j] = socketWebcam[j + 1];
        socketPrincipalCliente[j] = socketPrincipalCliente[j + 1];
        socketArchivosCliente[j] = socketArchivosCliente[j + 1];
        socketEscritorioCliente[j] = socketEscritorioCliente[j + 1];
        socketWebcamCliente[j] = socketWebcamCliente[j + 1];
      }
    //Disminuimos los contadores de conexiones
    conexiones1--;
    conexionesEscritorio1--;
    conexionesArchivos1--;
    conexionesWebcam1--;
    conexiones2--;
    conexionesEscritorio2--;
    conexionesArchivos2--;
    conexionesWebcam2--;

}
void MainWindow::conectadoEscritorio()
{
    socketEscritorio[conexionesEscritorio1] = new QTcpSocket(this);
    socketEscritorio[conexionesEscritorio1] = servidorEscritorio.nextPendingConnection();
    connect ( socketEscritorio[conexionesEscritorio1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorio() ) );
    conexionesEscritorio1++;
}
void MainWindow::conectadoWebcam()
{
    socketWebcam[conexionesWebcam1] = new QTcpSocket(this);
    socketWebcam[conexionesWebcam1] = servidorWebcam.nextPendingConnection();
    connect ( socketWebcam[conexionesWebcam1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcam() ) );
    conexionesWebcam1++;
}
void MainWindow::conectadoArchivos()
{
    socketArchivos[conexionesArchivos1] = new QTcpSocket(this);
    socketArchivos[conexionesArchivos1] = servidorArchivos.nextPendingConnection();
    connect ( socketArchivos[conexionesArchivos1],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivos() ) );
    conexionesArchivos1++;
}
void MainWindow::conectadoDemoxy()
{
        socketDemoxy = servidorDemoxy.nextPendingConnection();
        connect (socketDemoxy,SIGNAL(readyRead()),this,SLOT(llegadaDatosDemoxy()));
        connect (socketDemoxy,SIGNAL(disconnected()),this,SLOT(desconectadoDemoxy()));
        QByteArray listaConexiones;
        listaConexiones.setNum(conexiones1);
        socketDemoxy->write("servidores|@|" + listaConexiones);
}
void MainWindow::desconectadoDemoxy()
{
    conexiones2 = 0;
    conexionesArchivos2 = 0;
    conexionesEscritorio2 = 0;
    conexionesWebcam2 = 0;

}
void MainWindow::conectadoPrincipalCliente()
{
    socketPrincipalCliente[conexiones2] = new QTcpSocket(this);
    socketPrincipalCliente[conexiones2] = servidorPrincipalCliente.nextPendingConnection();
    connect ( socketPrincipalCliente[conexiones2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosPrincipalCliente() ) );
    connect ( socketPrincipalCliente[conexiones2],SIGNAL(disconnected()),this,SLOT(desconectadoPrincipalCliente()));
    conexiones2++;
}
void MainWindow::desconectadoPrincipalCliente()
{
    //A implementar
}
void MainWindow::conectadoEscritorioCliente()
{
    socketEscritorioCliente[conexionesEscritorio2] = new QTcpSocket(this);
    socketEscritorioCliente[conexionesEscritorio2] = servidorEscritorioCliente.nextPendingConnection();
    connect ( socketEscritorioCliente[conexionesEscritorio2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosEscritorioCliente() ) );
    conexionesEscritorio2++;
}
void MainWindow::conectadoWebcamCliente()
{
    socketWebcamCliente[conexionesWebcam2] = new QTcpSocket(this);
    socketWebcamCliente[conexionesWebcam2] = servidorWebcamCliente.nextPendingConnection();
    connect ( socketWebcamCliente[conexionesWebcam2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosWebcamCliente() ) );
    conexionesWebcam2++;
}
void MainWindow::conectadoArchivosCliente()
{
    socketArchivosCliente[conexionesArchivos2] = new QTcpSocket(this);
    socketArchivosCliente[conexionesArchivos2] = servidorArchivosCliente.nextPendingConnection();
    connect ( socketArchivosCliente[conexionesArchivos2],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatosArchivosCliente() ) );
    conexionesArchivos2++;
}


void MainWindow::llegadaDatosPrincipal()
{
    for(activo=0;activo<conexiones1;activo++)
    {
        if (socketPrincipal[activo]->bytesAvailable() > 0)
        {
            socketPrincipalCliente[activo]->write(socketPrincipal[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosEscritorio()
{
    for(activo=0;activo<conexionesEscritorio1;activo++)
    {
        if (socketEscritorio[activo]->bytesAvailable() > 0)
        {
            socketEscritorioCliente[activo]->write(socketEscritorio[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosWebcam()
{
    for(activo=0;activo<conexionesWebcam1;activo++)
    {
        if (socketWebcam[activo]->bytesAvailable() > 0)
        {
            socketWebcamCliente[activo]->write(socketWebcam[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosArchivos()
{
    for(activo=0;activo<conexionesArchivos1;activo++)
    {
        if (socketArchivos[activo]->bytesAvailable() > 0)
        {
            socketArchivosCliente[activo]->write(socketArchivos[activo]->readAll());
        }
    }
}

void MainWindow::llegadaDatosPrincipalCliente()
{
    for(activo=0;activo<conexiones2;activo++)
    {
        if(socketPrincipalCliente[activo]->bytesAvailable() > 0)
        {
            socketPrincipal[activo]->write(socketPrincipalCliente[activo]->readAll());
        }
    }

}
void MainWindow::llegadaDatosEscritorioCliente()
{
    for(activo=0;activo<conexionesEscritorio2;activo++)
    {
        if(socketEscritorioCliente[activo]->bytesAvailable() > 0)
        {
            socketEscritorio[activo]->write(socketEscritorioCliente[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosWebcamCliente()
{
    for(activo=0;activo<conexionesWebcam2;activo++)
    {
        if(socketWebcamCliente[activo]->bytesAvailable() > 0)
        {
            socketWebcam[activo]->write(socketWebcamCliente[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosArchivosCliente()
{
    for(activo=0;activo<conexionesArchivos2;activo++)
    {
        if(socketArchivosCliente[activo]->bytesAvailable() > 0)
        {
            socketArchivos[activo]->write(socketArchivosCliente[activo]->readAll());
        }
    }
}
void MainWindow::llegadaDatosDemoxy()
{

}
