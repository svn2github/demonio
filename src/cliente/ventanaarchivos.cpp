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
void ventanaArchivos::ponerArchivo ( QString archivo )
{
  //QIcon icono;
  //icono.addFile("drive-harddisk.png");
  ui->archivosLista->addItem ( archivo );
  //ui->archivosLista->item(ui->archivosLista->count() - 1)->setIcon(icono);
}
void ventanaArchivos::ponerDirectorio ( QString directorio )
{
  //QIcon icono;
  //icono.addFile("drive-harddisk.png");
  ui->directoriosLista->addItem ( directorio );
  //ui->directoriosLista->item(ui->directoriosLista->count() - 1)->setIcon(icono);
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

