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

void reconstruccion::procesarImagen(QByteArray captura,QImage imagen1)
{
    captura = qUncompress(captura);
    int i,j;
    QImage imagen2;
    imagen2.loadFromData ( captura,"jpg" );
    for(i=0;i<imagen1.width();i++)
    {
        for(j=0;j<imagen1.height();j++)
        {

            if(!(qRed(imagen2.pixel(i,j)) > 190 && qBlue(imagen2.pixel(i,j)) > 190) || qGreen(imagen2.pixel(i,j)) > 182)
            {
                imagen1.setPixel(i,j,imagen2.pixel(i,j));
            }
        }
    }
    emit imagen(imagen1);
}

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
  connect ( ui->botonIniciar,SIGNAL ( clicked() ),this,SLOT ( checkStreaming() ) );
  connect (ui->botonPantallaCompleta,SIGNAL(clicked()),this,SLOT(maximizar()));
  connect (ui->spinIntervalo,SIGNAL(valueChanged(int)),this,SLOT(ponerTiempo()));
  connect (&refresco,SIGNAL(timeout()),this,SLOT(botonCapturar()));
  connect (&reco,SIGNAL(imagen(QImage)),this,SLOT(ponerCaptura(QImage)));
  connect (this,SIGNAL(procesar(QByteArray,QImage)),&reco,SLOT(procesarImagen(QByteArray,QImage)));
  ui->horizontalLayout_2->insertWidget(1,imageEscritorio);
  imageEscritorio->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  interruptor = false;
  reco.moveToThread(&hilo);
  hilo.start();
}

void ventanaEscritorio::maximizar()
{
    util.ventanaEmergente(tr("Va a entrar en módo pantalla completa, para salir pulse la tecla de Windows"));
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
       QString tecla1;
       tecla1.setNum(teclado->key());
       emit tecla("t|@|" + tecla1 + "|@|");
    }
}
void ventanaEscritorio::mousePressEvent(QMouseEvent *boton)
{
    if(this->img->isFullScreen() && ui->checkTeclado->isChecked()) //Capturamos los clicks de raton cuando estamos a pantalla completa y el check esta activado
    {
        //Como puede haber diferentes resoluciones hay que calcular la equivalencia de un click en nuestro monitor con la del monitor remoto
        QString x;
        x.setNum((int)((((float)boton->globalX() / QApplication::desktop()->width())) * this->ancho));
        QString y;
        y.setNum((int)((((float)boton->globalY() / QApplication::desktop()->height())) * this->alto));

        if(boton->button() == Qt::LeftButton)
        {
            emit click("izqp|@|" + x + "|@|" + y); //Emitimos un signal personalizado
        }
        if(boton->button() == Qt::RightButton)
        {
            emit click("derp|@|" + x + "|@|" + y);
        }

    }
}

void ventanaEscritorio::mouseReleaseEvent(QMouseEvent *boton)
{
    if(this->img->isFullScreen() && ui->checkTeclado->isChecked()) //Capturamos los clicks de raton cuando estamos a pantalla completa y el check esta activado
    {
        //Como puede haber diferentes resoluciones hay que calcular la equivalencia de un click en nuestro monitor con la del monitor remoto
        QString x;
        x.setNum((int)((((float)boton->globalX() / QApplication::desktop()->width())) * this->ancho));
        QString y;
        y.setNum((int)((((float)boton->globalY() / QApplication::desktop()->height())) * this->alto));

        if(boton->button() == Qt::LeftButton)
        {
            emit click("izqs|@|" + x + "|@|" + y); //Emitimos un signal personalizado
        }
        if(boton->button() == Qt::RightButton)
        {
            emit click("ders|@|" + x + "|@|" + y);
        }

    }
}

void ventanaEscritorio::botonCapturar()
{
  refresco.stop();
  disconnect ( socketEscritorio[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
  connect ( socketEscritorio[activo],SIGNAL ( readyRead() ),this,SLOT ( llegadaDatos() ) );
  QString calidad;
  calidad.setNum ( ui->sliderCalidad->value() );
  util.escribirSocket (calidad,socketEscritorio[activo] );
}
void ventanaEscritorio::botonGuardar()
{
  guardarCaptura ( QFileDialog::getSaveFileName ( this,"Guardar captura" ),*captura1);
}
void ventanaEscritorio::cambioCalidad()
{
  ui->lcdCalidad->display ( ui->sliderCalidad->value() );
}
void ventanaEscritorio::checkStreaming()
{
      if(interruptor)
      {
        interruptor = false;
        ui->botonIniciar->setText(tr("Iniciar"));
        refresco.stop();
      }
      else
      {
        ui->botonIniciar->setText(tr("Parar"));
        ponerTiempo();
        interruptor = true;
        refresco.start();
      }
}

void ventanaEscritorio::ponerTiempo()
{
    refresco.setInterval(ui->spinIntervalo->value());
}
void ventanaEscritorio::ponerCaptura (QImage imagen1)
{
      QPixmap imagen;
      imagen = captura1->fromImage(imagen1,Qt::ColorOnly);
      *captura1 = imagen;
      imagen = imagen.scaled ( QApplication::desktop()->size());
      img->setPixmap ( imagen );

      imagen = imagen.scaled ( imageEscritorio->size(),Qt::KeepAspectRatio );
      imageEscritorio->setPixmap ( imagen );
      if (ui->checkGuardar->isChecked()) //Si hay que guardar la captura
      {
        QString capGuarda;
        this->numCapturas++;
        capGuarda.setNum(this->numCapturas);
        guardarCaptura(capGuarda + ".jpg",*captura1);
      }
      if(interruptor)
        refresco.start();

}
void ventanaEscritorio::guardarCaptura ( QString rutaArchivo,QPixmap captura )
{
    captura.save(rutaArchivo,"jpeg",100);
}
void ventanaEscritorio::llegadaDatos()
{

  //refresco.stop();
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
          emit procesar(datos,captura1->toImage());
          tamano = 0;
        }
      else
        {
          if(socketEscritorio[activo]->bytesAvailable() > tamano ) //Si se pasa algo ha ido mal, descartamos esa captura
          {
            datos = socketEscritorio[activo]->readAll();
            tamano = 0;
            if(interruptor)
              refresco.start();
          }

        }
    }
}
