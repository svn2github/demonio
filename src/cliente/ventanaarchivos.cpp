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

#include "ventanaarchivos.h"
#include "ui_ventanaArchivos.h"

ventanaArchivos::ventanaArchivos ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::ventanaArchivos )
{
  ui->setupUi ( this );
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
    QString extension = archivo.split(".").last();
        icono.addFile(":/icons/tiposmime/desconocido.png");
    if (extension.toLower() == "jpg" || extension.toLower() == "jpeg" || extension.toLower() == "png" || extension.toLower() == "gif" || extension.toLower() == "bmp")
        icono.addFile(":/icons/tiposmime/imagen.png");
    if (extension.toLower() == "mp3" || extension.toLower() == "wav" || extension.toLower() == "ogg" || extension.toLower() == "wma")
        icono.addFile(":/icons/tiposmime/audio.png");
    if (extension.toLower() == "mp4" || extension.toLower() == "webm" || extension.toLower() == "avi" || extension.toLower() == "flv" || extension.toLower() == "mpeg" || extension.toLower() == "wmv")
        icono.addFile(":/icons/tiposmime/video.png");
    if (extension.toLower() == "exe" || extension.toLower() == "com" || extension.toLower() == "bat" || extension.toLower() == "jar" || extension.toLower() == "msi" || extension.toLower() == "sh")
        icono.addFile(":/icons/tiposmime/ejecutable.png");
    if (extension.toLower() == "txt" || extension.toLower() == "doc" || extension.toLower() == "odt")
        icono.addFile(":/icons/tiposmime/txt.png");
    if (extension.toLower() == "pdf")
        icono.addFile(":/icons/tiposmime/pdf.png");
    if (extension.toLower() == "zip" || extension.toLower() == "rar" || extension.toLower() == "gz" || extension == "7z")
        icono.addFile(":/icons/tiposmime/comprimido.png");

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
  icono.addFile(":/icons/tiposmime/carpeta.png");
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
      break;
    }
    case QEvent::Show:
    {
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
QLabel *ventanaArchivos::labelMiniatura()
{
    return ui->labelMiniatura;
}
QProgressBar *ventanaArchivos::barraProgresoTransferencia()
{
    return ui->progresoTransferencia;
}
void ventanaArchivos::limpiarArchivos()
{
  ui->archivosLista->clear();
  ui->directoriosLista->clear();
}

