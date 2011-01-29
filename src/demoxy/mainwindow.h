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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int conexiones1;
    int conexionesArchivos1;
    int conexionesEscritorio1;
    int conexionesWebcam1;
    int conexiones2;
    int conexionesArchivos2;
    int conexionesEscritorio2;
    int conexionesWebcam2;
    int activo;
    QTcpServer servidorPrincipal;
    QTcpServer servidorEscritorio;
    QTcpServer servidorWebcam;
    QTcpServer servidorArchivos;
    QTcpServer servidorDemoxy;

    QTcpSocket *socketPrincipal[100];
    QTcpSocket *socketEscritorio[100];
    QTcpSocket *socketWebcam[100];
    QTcpSocket *socketArchivos[100];
    QTcpSocket *socketDemoxy;

    QTcpServer servidorPrincipalCliente;
    QTcpServer servidorEscritorioCliente;
    QTcpServer servidorWebcamCliente;
    QTcpServer servidorArchivosCliente;

    QTcpSocket *socketPrincipalCliente[100];
    QTcpSocket *socketEscritorioCliente[100];
    QTcpSocket *socketWebcamCliente[100];
    QTcpSocket *socketArchivosCliente[100];
public slots:
    void conectadoPrincipal();
    void conectadoEscritorio();
    void conectadoWebcam();
    void conectadoArchivos();
    void conectadoPrincipalCliente();
    void conectadoEscritorioCliente();
    void conectadoWebcamCliente();
    void conectadoArchivosCliente();
    void conectadoDemoxy();
    void desconectadoDemoxy();
    void desconectadoPrincipal();
    void desconectadoPrincipalCliente();
    void llegadaDatosPrincipal();
    void llegadaDatosEscritorio();
    void llegadaDatosWebcam();
    void llegadaDatosArchivos();
    void llegadaDatosPrincipalCliente();
    void llegadaDatosEscritorioCliente();
    void llegadaDatosWebcamCliente();
    void llegadaDatosArchivosCliente();
    void llegadaDatosDemoxy();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
