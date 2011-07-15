#include "ventanaopcionesxmpp.h"
#include "ui_ventanaopcionesxmpp.h"

ventanaopcionesxmpp::ventanaopcionesxmpp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ventanaopcionesxmpp)
{
    ui->setupUi(this);
    QIcon icono;
    icono.addFile(":/icons/preferencias.png");
    this->setWindowIcon(icono);
    connect(ui->botonEstablecer,SIGNAL(clicked()),this,SLOT(establecer()));
}

ventanaopcionesxmpp::~ventanaopcionesxmpp()
{
    delete ui;
}

void ventanaopcionesxmpp::establecer()
{
    QString config = "";
    if(ui->checkEnBanda->isChecked())
    {
        config = "banda|@|";
    }
    else
    {
        config =  "nobanda|@|";
    }
    if(ui->textoProxy->text() == "")
    {
        config = config + "noproxy";
    }
    else
    {
        config = config + ui->textoProxy->text();
    }
    cliente->sendMessage(servidor,"establecerxmpp|@|" + config);
}
