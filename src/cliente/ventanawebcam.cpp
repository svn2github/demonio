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

#include "ventanawebcam.h"
#include "ui_ventanawebcam.h"
#include <QFileDialog>

ventanawebcam::ventanawebcam(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ventanawebcam)
{
    ui->setupUi(this);
    calidad = 100;
    connect (ui->barraCalidad,SIGNAL(valueChanged(int)),this,SLOT(cambioCalidad()));
    connect (ui->botonCapturarwebcam,SIGNAL(clicked()),this,SLOT(capturar()));
    connect (ui->botonEncenderWebcam,SIGNAL(clicked()),this,SLOT(encender()));
    connect (ui->botonApagarWebcam,SIGNAL(clicked()),this,SLOT(apagar()));
    connect (ui->botonGuardarCaptura,SIGNAL(clicked()),this,SLOT(guardarCaptura()));
}

ventanawebcam::~ventanawebcam()
{
    delete ui;
}

void ventanawebcam::cambioCalidad()
{
    this->calidad = ui->barraCalidad->value();
    ui->lcdCalidad->display(this->calidad);
}

void ventanawebcam::capturar()
{
    QString tramaCaptura;
    QString cadenaCalidad;
    cadenaCalidad.setNum(this->calidad);
    tramaCaptura = "cap|@|" + cadenaCalidad;
    cliente->sendMessage(servidor,tramaCaptura);
}

void ventanawebcam::encender()
{
    cliente->sendMessage(servidor,"encender|@|");
}
void ventanawebcam::apagar()
{
    cliente->sendMessage(servidor,"apagar|@|");
}

void ventanawebcam::guardarCaptura()
{
    ui->imagenWebcam->pixmap()->save(QFileDialog::getSaveFileName(this,"Guardar como"),"jpeg",100);
}

QCheckBox *ventanawebcam::guardarAutomaticamente()
{
    return ui->checkGuardarAutomaticamente;
}
QCheckBox *ventanawebcam::capturasAutomaticas()
{
    return ui->checkCapturaAutomatica;
}
QLabel *ventanawebcam::imagenWebcam()
{
    return ui->imagenWebcam;
}

