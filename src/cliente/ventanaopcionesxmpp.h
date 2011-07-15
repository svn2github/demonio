#ifndef VENTANAOPCIONESXMPP_H
#define VENTANAOPCIONESXMPP_H

#include <QMainWindow>
#include <qxmpp/QXmppClient.h>

namespace Ui {
    class ventanaopcionesxmpp;
}

class ventanaopcionesxmpp : public QMainWindow
{
    Q_OBJECT

public:
    explicit ventanaopcionesxmpp(QWidget *parent = 0);
    ~ventanaopcionesxmpp();
    QString servidor;
    QXmppClient *cliente;
public slots:
    void establecer();
private:
    Ui::ventanaopcionesxmpp *ui;
};

#endif // VENTANAOPCIONESXMPP_H
