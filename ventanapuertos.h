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


#ifndef VENTANAPUERTOS_H
#define VENTANAPUERTOS_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
    class ventanaPuertos;
}

class ventanaPuertos : public QMainWindow
{
    Q_OBJECT

public:
    explicit ventanaPuertos(QWidget *parent = 0);
    ~ventanaPuertos();
    QLineEdit *puertoPrincipal();
    QLineEdit *puertoArchivos();
    QLineEdit *puertoEscritorio();
    QLineEdit *puertoWebcam();
    QPushButton *botonPonerPuertos();
private:
    Ui::ventanaPuertos *ui;
};

#endif // VENTANAPUERTOS_H
