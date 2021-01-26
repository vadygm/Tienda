#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
}

Info::~Info()
{
    delete ui;
}

QString Info::dato() const
{
    return m_dato;
}

void Info::setDato(const QString &dato)
{
    m_dato = dato;
}

void Info::actualizar()
{
     ui->outDialogo->setPlainText(m_dato);
}
