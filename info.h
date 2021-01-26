#ifndef INFO_H
#define INFO_H

#include <QDialog>

namespace Ui {
class Info;
}

class Info : public QDialog
{
    Q_OBJECT

public:
    explicit Info(QWidget *parent = nullptr);
    ~Info();

    QString dato() const;
    void setDato(const QString &dato);
    void actualizar();

private:
    Ui::Info *ui;
    QString m_dato;
};

#endif // INFO_H
