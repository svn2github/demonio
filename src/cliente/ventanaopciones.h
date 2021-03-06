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

#ifndef VENTANAOPCIONES_H
#define VENTANAOPCIONES_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
namespace Ui {
    class ventanaOpciones;
}

class ventanaOpciones : public QMainWindow {
    Q_OBJECT
public:
    ventanaOpciones(QWidget *parent = 0);
    ~ventanaOpciones();
    QMessageBox mensaje;
public slots:
    void mostrarContrasena();
    void examinar();
    void activarJoiner();
    void activarEjecucion();
    void activarInicio();
    void activarMensaje();
    void activarProteger();
    void crearServidor();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::ventanaOpciones *ui;
};

#endif // VENTANAOPCIONES_H
