#include "tienda.h"
#include "ui_tienda.h"
#include "acerca.h"
#include "info.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QRegExpValidator>

#define NOMBRE_RX "^(?!\\s)([,.'-]+)$//i+-*[]();:.#!$%&/()=?¡P*][´+}{´+1234567890"

Tienda::Tienda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tienda)
{
    ui->setupUi(this);

    m_subtotal = 0;

    cargarDatos();
    inicilizarWidgets();

}

Tienda::~Tienda()
{
    delete ui;
}

void Tienda::mostrarPrecio(int indice)
{
    // obtener el producto que se ha seleccionado en el combo
    float precioProducto = m_productos.at(indice)->precio();

    // Mostrar el precio en el widget correspondiente
    ui->outPrecio->setText("$ " + QString::number(precioProducto));
}

void Tienda::cargarDatos()
{
    // Crear y agregar productos a la lista
    //m_productos.append(new Producto(1,"Pan", 0.15));
    //m_productos.append(new Producto(2,"Leche", 0.80));
    //m_productos.append(new Producto(3,"Huevos", 0.12));

    QString pathActual = QDir::currentPath();
    QFile archivo(pathActual + "/productos.csv");

    bool primeraLinea = true;
    if (archivo.open(QFile::ReadOnly)){
        QTextStream in(&archivo);
        while(!in.atEnd()){
            QString linea = in.readLine();
            if (primeraLinea){
                primeraLinea = false;
                continue;
            }
            // separar los datos por ','
            QStringList datos = linea.split(";");
            int codigo = datos[0].toInt();
            float precio = datos[2].toFloat();
            // Crear producto
            m_productos.append(new Producto(codigo,datos[1],precio));
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("El archivo de prodfuctos no se puede abrir.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }

}

void Tienda::inicilizarWidgets()
{
    // Agrega los productos al combo box
    for(int i=0; i < m_productos.length(); i++){
        ui->inProducto->addItem(m_productos.at(i)->nombre());
    }

    // Colocar cabecera a la tabla
    QStringList cabecera = {"Cantidad", "Nombre", "Sutotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);

    // Eventos
    connect(ui->inProducto, SIGNAL(currentIndexChanged(int)),
            this, SLOT(mostrarPrecio(int)));

    // Mostrr el precio del primer producto
    mostrarPrecio(0);


}

void Tienda::calcular(float subtotal)
{
    // Calcular valores
    m_subtotal += subtotal;
    float iva = m_subtotal * IVA;
    float total = m_subtotal + iva;
    // Mostrar valores en UI
    ui->outSubtotal->setText(QString::number(m_subtotal,'f',2));
    ui->outIva->setText(QString::number(iva,'f',2));
    ui->outTotal->setText(QString::number(total,'f',2));

}

void Tienda::validarCedula()
{
    QString cedula = ui->inCedula->text();
    if (cedula.length() != 10){
        ui->inCedula->setStyleSheet("background-color: rgb(239, 41, 41);");
    }else{
        ui->inCedula->setStyleSheet("background-color: rgb(132, 225, 113);");
    }

}


void Tienda::on_cmdAgregar_clicked()
{
    // Obtener el producto seleccionado
    int indice = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(indice);
    // Obtener la cantidad
    int cantidad = ui->inCantidad->value();

    // Validar: si la cantidad es 0, no hace nada
    if (cantidad == 0){
        return;
    }
    // Calcular el subtotal
    float subtotal = cantidad * p->precio();

    // Agregar a la tabla
    int posicion = ui->outDetalle->rowCount();
    ui->outDetalle->insertRow(posicion);
    ui->outDetalle->setItem(posicion,0,new QTableWidgetItem(QString::number(cantidad,'f',2)));
    ui->outDetalle->setItem(posicion,1,new QTableWidgetItem(p->nombre()));
    ui->outDetalle->setItem(posicion,2,new QTableWidgetItem(QString::number(subtotal,'f',2)));

    // Limpiar widgets
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();

    // calcular
    calcular(subtotal);


}

void Tienda::boton()
{
    Info *informacion= new Info();
    QString info=ui->actionInformacion->text();
    info="Nuestra aplicacion consiste de un menu claro, sencillo y eficaz. Disfrutala";
    informacion->setDato(info);

}
void Tienda::on_cmdFinalizar_clicked()
{
    Acerca *dialogAcerca= new Acerca();
    QString nombre=ui->inNombre->text();
    QString cedula=ui->inCedula->text();
    QString telefono=ui->inTelefono->text();
    QString email=ui->inEmail->text();
    QString producto=ui->inProducto->currentText();
    int cantidad=ui->inCantidad->value();
    QString subtotal=ui->outSubtotal->text();
    QString total=ui->outTotal->text();
    QString iva=ui->outIva->text();
   // QString cantidad=ui->inCantidad->value();
    QString datos="   Nombre: "+nombre+"\n"+
                  "   Cedula: "+cedula+"\n"
                  "   Telefono: "+telefono+"\n"
                  "   E-mail: "+email+"\n"
                  "   Detalles:"+"\n"
                  "   Productos :"+producto+"\n"
                  "   Cantidad :"+cantidad+"\n"
                        +"\t" "Subtotal:"+subtotal+"\n"
                        +"\t" "iva:"+iva+"\n"
                        +"\t" "Total:"+total;


    dialogAcerca->setDato(datos);
    dialogAcerca->actualizar();
    dialogAcerca->show();
    QStringList cabecera = {"Cantidad", "Nombre", "Sutotal"};
    ui->outDetalle->setColumnCount(3);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    QAbstractItemModel* const mdl = ui->outDetalle->model();
    mdl->removeRows(0,mdl->rowCount());
    ui->inNombre->clear();
    ui->inCedula->clear();
    ui->inEmail->clear();
    ui->inTelefono->clear();
    ui->inDireccion->clear();
    ui->outIva->clear();
    ui->outSubtotal->clear();
    ui->outTotal->clear();

}

void Tienda::on_inCedula_editingFinished()
{
    validarCedula();
}

void Tienda::on_inNombre_textEdited(const QString &arg1)
{
    QString texto=arg1;
    QString nombre(NOMBRE_RX);
    for(int i=0;i<texto.size();i++){
        foreach (const QString &y,nombre){
            if(texto.at(i)==y){
                texto[i]=' ';
            ui->inNombre->setText(texto);
            }
        }
    }
}

void Tienda::on_cmdAgregar_released()
{
    cargarDatos();
}
