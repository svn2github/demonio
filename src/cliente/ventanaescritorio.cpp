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

#include "ventanaescritorio.h"
#include "ui_ventanaescritorio.h"
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QBuffer>

void reconstruccion::procesarImagen(QByteArray captura)
{
    /** Esta función procesa los datos pasados como parametro como una imagen de diferencia y reconstruye la imagen final usando la captura anterior **/
    int i,j;
    QImage imagen2;
    imagen2.loadFromData (captura,"jpg" ); //Cargamos el array de bytes como una imagen
    for(i=0;i<imagen1->width();i++) //Recorremos la imagen
    {
        for(j=0;j<imagen1->height();j++)
        {

            if(!(qRed(imagen2.pixel(i,j)) > 180 && qBlue(imagen2.pixel(i,j)) > 180) || qGreen(imagen2.pixel(i,j)) > 140)
            { /*Si cada valor de rojo,azul y verde de un pixel esta entre estos valores, entonces es el color que consideramos transparente
                    hay que hacerlo asi porque al pasar a jpeg como es compresion con perdida el color que habiamos establecido como transparente
                    puede variar ligeramente */
               imagen1->setPixel(i,j,imagen2.pixel(i,j));
             }
         }
     }
     emit imagen(*imagen1); //Emitimos una señal con la imagen reconstruida */
 }


ventanaEscritorio::ventanaEscritorio ( QWidget *parent ) :
    QMainWindow ( parent ),
    ui ( new Ui::ventanaEscritorio )
{
  ui->setupUi ( this );
  this->imageEscritorio = new QLabel(this);
  this->img = new QLabel(0);
  this->imageEscritorio->setScaledContents(true);
  captura1 = new QPixmap();
  numCapturas = 0;
  alto = 0;
  ancho = 0;
  connect ( ui->sliderCalidad,SIGNAL ( valueChanged ( int ) ),this,SLOT ( cambioCalidad() ) );
  connect ( ui->botonCapturar,SIGNAL ( clicked() ),this,SLOT ( botonCapturar() ) );
  connect (&reco,SIGNAL(error()),this,SLOT(botonCapturar()));
  connect ( ui->botonGuardar,SIGNAL ( clicked() ),this,SLOT ( botonGuardar() ) );
  connect ( ui->botonIniciar,SIGNAL ( clicked() ),this,SLOT ( checkStreaming() ) );
  connect (ui->botonPantallaCompleta,SIGNAL(clicked()),this,SLOT(maximizar()));
  connect (ui->spinIntervalo,SIGNAL(valueChanged(int)),this,SLOT(ponerTiempo()));
  connect (&refresco,SIGNAL(timeout()),this,SLOT(botonCapturar()));
  connect (&reco,SIGNAL(imagen(QImage)),this,SLOT(ponerCaptura(QImage)));
  ui->horizontalLayout_2->insertWidget(1,imageEscritorio);
  imageEscritorio->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  interruptor = false;
  reco.moveToThread(&hilo); //movemos esta clase a un nuevo hilo para no bloquear el programa principal al hacer los calculos de imagenes
  hilo.start();
  refresco.stop();
  teclas = "|@|";
}

void ventanaEscritorio::maximizar()
{
    /** Maximiza a pantalla completa y captura el raton y teclado **/
    //util.ventanaEmergente(tr("Va a entrar en módo pantalla completa, para salir pulse la tecla de Windows"));
    this->grabKeyboard();
    this->grabMouse();
    this->img->showFullScreen();
}

void ventanaEscritorio::ocultar()
{
 /** Minimiza y suelta el raton y el teclado **/
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
    /** Se ejecuta al soltar una tecla **/
    if (teclado->key() == Qt::Key_Meta) //La tecla para salir de pantalla completa
    {
        this->ocultar();
    }

    if(this->img->isFullScreen() && ui->checkTeclado->isChecked())
    {
       //Si estamos a pantalla completa y esta activado el check de teclado y raton podemos enviar
       QString tec;
       tec.setNum(teclado->key());
       teclas = teclas + tec + "|@|";
       refresco.start(); //Reiniciamos el timer de las capturas porque sino se queda pillado a veces
    }

}
void ventanaEscritorio::mousePressEvent(QMouseEvent *boton)
{

    if(this->img->isFullScreen() && ui->checkTeclado->isChecked()) //Capturamos los clicks de raton cuando estamos a pantalla completa y el check esta activado
    {
        //Simplemente guardamos la posicion que tenia el raton al pulsar un boton para ver si se ha arrastrado o es un click
        arrastrax = boton->globalX();
        arrastray = boton->globalY();
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
        if (boton->globalX() != arrastrax || boton->globalY() != arrastray) //Si la posicion donde se pulso un boton es distinta de donde se suelta es arrastrar
        {
            QString arrax;
            arrax.setNum((int)((((float)arrastrax / QApplication::desktop()->width())) * this->ancho));
            QString array;
            array.setNum((int)((((float)arrastray / QApplication::desktop()->height())) * this->alto));
            emit click("arra|@|" + arrax + "|@|" + array + "|@|" + x + "|@|" + y);
        }
        else
        {
            if(boton->button() == Qt::LeftButton)
            {
                emit click("izq|@|" + x + "|@|" + y); //Emitimos un signal personalizado
            }
            if(boton->button() == Qt::RightButton)
            {
                emit click("der|@|" + x + "|@|" + y);
            }
        }
        refresco.start(); //Lo mismo que en la funcion anterior
    }
}

void ventanaEscritorio::botonCapturar()
{
    /** Manda a hacer una captura **/
    refresco.stop(); //No mas capturas hasta que llege esta
    if(teclas != "|@|") //Enviamos las teclas
        emit tecla("t" + teclas);
    teclas = "|@|";
    QString calidad;
    calidad.setNum ( ui->sliderCalidad->value() );
    cliente->sendMessage(servidor,"captura|@|"+calidad);
}
void ventanaEscritorio::botonGuardar()
{
  guardarCaptura ( QFileDialog::getSaveFileName ( this,"Guardar captura" ),*img->pixmap());
}
void ventanaEscritorio::cambioCalidad()
{
  /** Establece la calidad  de las capturas segun esté la barra de calidad **/
  ui->lcdCalidad->display ( ui->sliderCalidad->value() );
}
void ventanaEscritorio::checkStreaming()
{
    /** Inicia las capturas seguidas **/
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
    /** Lee la caja de valores y pone el tiempo del timer**/
    refresco.setInterval(ui->spinIntervalo->value());
}
void ventanaEscritorio::ponerCaptura (QImage imagen1)
{ 
    /** Hace conversiones con la imagen para mostrarla en pantalla **/
      QPixmap imagen;
      imagen = captura1->fromImage(imagen1,Qt::ColorOnly);
      imagen = imagen.scaled ( QApplication::desktop()->size());
      img->setPixmap ( imagen );

      imagen = imagen.scaled ( imageEscritorio->size(),Qt::KeepAspectRatio );
      imageEscritorio->setPixmap ( imagen );
      if (ui->checkGuardar->isChecked()) //Si hay que guardar la captura
      {
        QString capGuarda;
        this->numCapturas++;
        capGuarda.setNum(this->numCapturas);
        guardarCaptura(capGuarda + ".jpg",*img->pixmap());
      }
      if(interruptor) //Cuando ha llegado la captura y si esta activado el boton de captura seguidas entonces seguimos pidiendo
          refresco.start();

}
void ventanaEscritorio::guardarCaptura ( QString rutaArchivo,QPixmap captura )
{
    /** Guarda la captura pasada como parámetro en la ruta también pasada como parámetro **/
    captura.save(rutaArchivo,"jpeg",100);
}

reconstruccion::reconstruccion()
{

}
