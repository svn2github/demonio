/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      21,   11,   11,   11, 0x0a,
      32,   11,   11,   11, 0x0a,
      47,   11,   11,   11, 0x0a,
      69,   11,   11,   11, 0x0a,
     102,   11,   94,   11, 0x0a,
     115,   11,   11,   11, 0x0a,
     146,  138,  130,   11, 0x0a,
     161,   11,   11,   11, 0x0a,
     178,  173,   11,   11, 0x0a,
     202,  173,   11,   11, 0x0a,
     247,  229,   11,   11, 0x0a,
     287,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0inicio()\0conectar()\0"
    "llegadaDatos()\0llegadaDatosArchivo()\0"
    "llegadaDatosEscritorio()\0QPixmap\0"
    "screenShot()\0desconectado()\0QString\0"
    "comando\0shell(QString)\0reiniciar()\0"
    "ruta\0listarArchivos(QString)\0"
    "listarDirectorios(QString)\0tipo,titulo,texto\0"
    "mostrarMensaje(QString,QString,QString)\0"
    "datosEscritos()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: inicio(); break;
        case 1: conectar(); break;
        case 2: llegadaDatos(); break;
        case 3: llegadaDatosArchivo(); break;
        case 4: llegadaDatosEscritorio(); break;
        case 5: { QPixmap _r = screenShot();
            if (_a[0]) *reinterpret_cast< QPixmap*>(_a[0]) = _r; }  break;
        case 6: desconectado(); break;
        case 7: { QString _r = shell((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 8: reiniciar(); break;
        case 9: listarArchivos((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: listarDirectorios((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: mostrarMensaje((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 12: datosEscritos(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
