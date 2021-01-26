#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "producto.h"
#include "acerca.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Tienda; }
QT_END_NAMESPACE

class Tienda : public QMainWindow
{
    Q_OBJECT

public:
    Tienda(QWidget *parent = nullptr);
    ~Tienda();
    void imprimir();

public slots:
    void mostrarPrecio(int indice);

private slots:
    void on_cmdAgregar_clicked();
    void on_cmdFinalizar_clicked();
    void on_inCedula_editingFinished();
    void on_inNombre_textEdited(const QString &arg1);
    void on_cmdAgregar_released();

private:
    Ui::Tienda *ui;
    QList<Producto*> m_productos;
    float m_subtotal;

    const float IVA = 0.12;

    void cargarDatos();
    void inicilizarWidgets();
    void calcular(float subtotal);
    void validarCedula();
    void boton();


};
#endif // TIENDA_H
