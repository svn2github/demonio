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

#include "ventanaescritorio.h"
#include "ui_ventanaescritorio.h"
#include <QKeyEvent>
#include <QDesktopWidget>

ventanaEscritorio::ventanaEscritorio ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::ventanaEscritorio )
{
  ui->setupUi ( this );
  this->imageEscritorio = new QLabel(this);
  this->img = new QLabel(0);
  this->imageEscritorio->setScaledContents(true);
  QEvent teclado(QEvent::MouseButtonPress);
  img->eventFilter(this,&teclado);
  activo = 0;
  conexiones = 0;
  socketEscritorio[activo] = new QTcpSocket ( this );
  tamano = 0;
  numCapturas = 0;
  alto = 0;
  ancho = 0;
  connect ( ui->sliderCalidad,SIGNAL ( valueChanged ( int ) ),this,SLOT ( cambioCalidad() ) );
  connect ( ui->botonCapturar,SIGNAL ( clicked() ),this,SLOT ( botonCapturar() ) );
  connect ( ui->botonGuardar,SIGNAL ( clicked() ),this,SLOT ( botonGuardar() ) );
  connect ( ui->checkStreaming,SIGNAL ( clicked() ),this,SLOT ( checkStreaming() ) );
  connect ( imageEscritorio,SIGNAL(botonDerecho(int,int)),this,SLOT(click(int,int)));
  connect (ui->botonPantallaCompleta,SIGNAL(clicked()),this,SLOT(maximizar()));
  ui->verticalLayout_2->insertWidget(0,imageEscritorio);
 imageEscritorio->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}

void ventanaEscritorio::maximizar()
{
    util.ventanaEmergente("Va a entrar en m�do pantalla completa, para salir pulse la tecla de Windows");
    this->grabKeyboard();
    this->grabMouse();
    this->img->showFullScreen();
}

void ventanaEscritorio::ocultar()
{

    this->releaseKeyboard();
    this->releaseMouse();
    this->img->showNormal();
    this->img->close();
}

ventanaEscritorio::~ventanaEscritorio()
{
  delete ui;
}

void ventanaEscritorio::changeEvent ( QEvent *e )
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


void ventanaEscritorio::keyReleaseEvent(QKeyEvent *teclado)
{
    if (teclado->key() == Qt::Key_Meta)
    {
        this->ocultar();
    }

    if(this->img->isFullScreen() && ui->checkTeclado->isChecked())
    {
       QString tecla;
       tecla.setNum(teclado->key());
       util.escribirSocket("tecla|@|" + tecla,socketEscritorio[activo]);
    }
}

void ventanaEscritorio::mouseReleaseEvent(QMouseEvent *boton)
{
    if(this->img->isFullScreen() && ui->checkTeclado->isChecked())
    {
        QString x;
        x.setNum((int)((((float)boton->globalX() / QApplication::desktop()->width())) * this->ancho));
        QString y;
        y.setNum((int)((((float)boton->globalY() / QApplication::desktop()->height())) * this->alto));

        if(boton->button() == Qt::LeftButton)
        {
            util.escribirSocket("izquierdo|@|" + x + "|@|" + y,socketEscritorio[activo]);
        }
        if(boton->button() == Qt::RightButton)
        {
            util.escribirSocket("derecho|@|" + x + "|@|" + y,socketEscritorio[activo]);
        }

    }
}

void ventanaEscritorio::obtenerResolucion()
{
    util.escribirSocket("resolucion|@|",socketEscritorio[activo]);
}

void ventanaEscritorio::botonCapturar()
{
  disconnect ( socketEscritorio[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
  connect ( socketEscritorio[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
  QString calidad;
  calidad.setNum ( ui->sliderCalidad->value() );
  util.escribirSocket ( "capturar|@|" + calidad,socketEscritorio[activo] );
}
void ventanaEscritorio::botonGuardar()
{
  guardarCaptura ( QFileDialog::getSaveFileName ( this,"Guardar captura" ),datos );
}
void ventanaEscritorio::cambioCalidad()
{
  ui->lcdCalidad->display ( ui->sliderCalidad->value() );
}
void ventanaEscritorio::checkStreaming()
{
  if ( ui->checkStreaming->isChecked() )
    {
      tamano = 0;
      botonCapturar();
    }
}
void ventanaEscritorio::ponerCaptura ( QByteArray captura )
{
      QPixmap imagen;
      imagen.loadFromData ( captura,"jpg" );

      imagen = imagen.scaled ( QApplication::desktop()->size());
      img->setPixmap ( imagen );

      imagen = imagen.scaled ( imageEscritorio->size(),Qt::KeepAspectRatio );
      imageEscritorio->setPixmap ( imagen );
      this->obtenerResolucion();
      QString alto;
      QString ancho;
      ancho.setNum(this->ancho);
      alto.setNum(this->alto);
      this->setWindowTitle("Escritorio remoto - Resolucion: " + ancho + " x " + alto);

}
void ventanaEscritorio::guardarCaptura ( QString rutaArchivo,QByteArray captura )
{
  QFile salida;
  salida.setFileName ( rutaArchivo );
  salida.open ( QIODevice::WriteOnly );
  salida.write ( captura );
}
void ventanaEscritorio::llegadaDatos()
{

  if ( tamano == 0 )
    {
      datos = util.leerSocketDatos ( socketEscritorio[activo] );
      tamano = datos.toInt();
    }
  else
    {
      if ( socketEscritorio[activo]->bytesAvailable() == tamano )
        {
          datos = socketEscritorio[activo]->readAll();
          ponerCaptura ( datos );
          tamano = 0;
          if (ui->checkGuardar->isChecked())
          {
            QString capGuarda;
            this->numCapturas++;
            capGuarda.setNum(this->numCapturas);
            guardarCaptura(capGuarda + ".jpg",datos);
          }
          if ( ui->checkStreaming->isChecked() )
          {
            botonCapturar();
          }
        }
      else
        {
          ui->progresoDescarga->setValue ( ( socketEscritorio[activo]->bytesAvailable() / tamano ) * 100 );
        }
    }
}