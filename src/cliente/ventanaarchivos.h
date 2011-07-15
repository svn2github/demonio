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

#ifndef VENTANAARCHIVOS_H
#define VENTANAARCHIVOS_H

#include <QMainWindow>
#include <QListWidget>
#include <QSignalMapper>
#include <QFile>
#include <QFileDialog>
#include <QComboBox>
#include <QProgressBar>
#include <QLabel>
namespace Ui
  {
  class ventanaArchivos;
}

class ventanaArchivos : public QMainWindow
  {
    Q_OBJECT
  public:
    ventanaArchivos ( QWidget *parent = 0 );
    ~ventanaArchivos();
    QString servidor;
    QString ruta;
    QString rutaAnterior;
    QString rutaArchivo;
    QListWidget *directoriosLista();
    QListWidget *archivosLista();
    QLineEdit *rutaTexto();
    QPushButton *botonIr();
    QPushButton *botonAtras();
    QPushButton *botonHome();
    QPushButton *botonRefresco();
    QPushButton *botonDescargar();
    QPushButton *botonSubir();
    QPushButton *botonCopiar();
    QPushButton *botonMover();
    QPushButton *botonEjecutar();
    QPushButton *botonCarpeta();
    QPushButton *botonRenombrar();
    QPushButton *botonBorrar();
    QPushButton *botonPrevia();
    QPushButton *botonBorrarCarpeta();
    QPushButton *botonTamano();
    QComboBox *comboUnidad();
    QLabel *labelMiniatura();
    QProgressBar *barraProgresoTransferencia();
    void establecerRuta ( QString ruta );
    void ponerUnidad(QString unidad);
    void ponerArchivo ( QString archivo );
    void ponerDirectorio ( QString directorio );
    void limpiarArchivos();
  protected:
    QIcon iconoArchivo(QString archivo);
    void changeEvent ( QEvent *e );
    bool event ( QEvent *event );
private:
    Ui::ventanaArchivos *ui;
  };

#endif // VENTANAARCHIVOS_H
