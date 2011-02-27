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

#include "ventanaarchivos.h"
#include "ui_ventanaArchivos.h"

ventanaArchivos::ventanaArchivos ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::ventanaArchivos )
{
  ui->setupUi ( this );
  activo = 0;
  conexiones = 0;
  socketArchivos[activo] = new QTcpSocket ( this );

}
ventanaArchivos::~ventanaArchivos()
{
  ui->archivosLista->clear();
  delete ui;

}
void ventanaArchivos::changeEvent ( QEvent *e )
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
void ventanaArchivos::establecerRuta ( QString ruta )
{
  ui->rutaTexto->setText ( ruta );
  this->ruta = ruta;
}
void ventanaArchivos::ponerUnidad(QString unidad)
{
    ui->comboUnidad->addItem( unidad );
}
QIcon ventanaArchivos::iconoArchivo(QString archivo)
{
    QIcon icono;
    QStringList cachos = archivo.split(".");
    QString extension = cachos[cachos.size() - 1];
        icono.addFile("./icons/tiposmime/desconocido.png");
    if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "bmp")
        icono.addFile("./icons/tiposmime/imagen.png");
    if (extension == "mp3" || extension == "wav" || extension == "ogg" || extension == "wma")
        icono.addFile("./icons/tiposmime/audio.png");
    if (extension == "mp4" || extension == "webm" || extension == "avi" || extension == "flv" || extension == "mpeg" || extension == "wmv")
        icono.addFile("./icons/tiposmime/video.png");
    if (extension == "exe" || extension == "com" || extension == "bat" || extension == "jar" || extension == "msi" || extension == "sh")
        icono.addFile("./icons/tiposmime/ejecutable.png");
    if (extension == "txt" || extension == "doc" || extension == "odt")
        icono.addFile("./icons/tiposmime/txt.png");
    if (extension == "pdf")
        icono.addFile("./icons/tiposmime/pdf.png");
    if (extension == "zip" || extension == "rar" || extension == "gz" || extension == "7z")
        icono.addFile("./icons/tiposmime/comprimido.png");

    return icono;
}
void ventanaArchivos::ponerArchivo ( QString archivo )
{ 
  ui->archivosLista->addItem ( archivo );
  ui->archivosLista->item(ui->archivosLista->count() - 1)->setIcon(iconoArchivo(archivo));
}
void ventanaArchivos::ponerDirectorio ( QString directorio )
{
  QIcon icono;
  icono.addFile("./icons/tiposmime/carpeta.png");
  ui->directoriosLista->addItem ( directorio );
  ui->directoriosLista->item(ui->directoriosLista->count() - 1)->setIcon(icono);
}
bool ventanaArchivos::event ( QEvent *event )
{
  switch ( event->type() )
    {
    case QEvent::Close:
    {
      ui->archivosLista->clear();
      ui->directoriosLista->clear();
      disconnect ( socketArchivos[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
      break;
    }
    case QEvent::Show:
    {
      connect ( socketArchivos[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
      ui->botonHome->click();
      // break;
    }
    default:
    {
      QMainWindow::event ( event );
      break;
    }
    }
  return true;
}
void ventanaArchivos::nuevaConexion()
{

}
void ventanaArchivos::llegadaDatos()
{
    ui->progresoTransferencia->setValue(util.recibirArchivo(rutaArchivo,socketArchivos[activo]));
    if (rutaArchivo == "mini.jpg" && ui->progresoTransferencia->value() == 100)
    {
        QPixmap imagen;
        imagen.load("mini.jpg");
        ui->labelMiniatura->setPixmap(imagen);
        rutaArchivo = "";
        ui->progresoTransferencia->setValue(0);
    }
}


QListWidget *ventanaArchivos::directoriosLista()
{
  return ui->directoriosLista;
}
QListWidget *ventanaArchivos::archivosLista()
{
  return ui->archivosLista;
}
QLineEdit *ventanaArchivos::rutaTexto()
{
  return ui->rutaTexto;
}
QPushButton *ventanaArchivos::botonIr()
{
  return ui->botonIr;
}
QPushButton *ventanaArchivos::botonAtras()
{
  return ui->botonAtras;
}
QPushButton *ventanaArchivos::botonHome()
{
  return ui->botonHome;
}
QPushButton *ventanaArchivos::botonRefresco()
{
  return ui->botonRefresco;
}
QPushButton *ventanaArchivos::botonDescargar()
{
  return ui->botonDescargar;
}
QPushButton *ventanaArchivos::botonSubir()
{
  return ui->botonSubir;
}
QPushButton *ventanaArchivos::botonBorrar()
{
  return ui->botonBorrar;
}
QPushButton *ventanaArchivos::botonEjecutar()
{
  return ui->botonEjecutar;
}
QPushButton *ventanaArchivos::botonCopiar()
{
  return ui->botonCopiar;
}
QPushButton *ventanaArchivos::botonMover()
{
  return ui->botonMover;
}
QPushButton *ventanaArchivos::botonCarpeta()
{
  return ui->bontonCarpeta;
}
QPushButton *ventanaArchivos::botonPrevia()
{
    return ui->botonPrevia;
}
QPushButton *ventanaArchivos::botonRenombrar()
{
    return ui->botonRenombrar;
}
QPushButton *ventanaArchivos::botonBorrarCarpeta()
{
    return ui->botonBorrarCarpeta;
}
QPushButton *ventanaArchivos::botonTamano()
{
    return ui->botonTamano;
}
QComboBox *ventanaArchivos::comboUnidad()
{
    return ui->comboUnidad;
}
void ventanaArchivos::subirArchivo ( QString archivo )
{
  util.enviarArchivo ( archivo,socketArchivos[activo] );
}
void ventanaArchivos::limpiarArchivos()
{
  ui->archivosLista->clear();
  ui->directoriosLista->clear();
}

