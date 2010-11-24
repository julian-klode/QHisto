
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include "JItemHistoModel.hpp"
#include "JHistoView.hpp"
#include <QStandardItemModel>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget wmain;
    QVBoxLayout layout;

    QStandardItemModel model;
    JHistoView view;
    JItemHistoModel hmodel;

    hmodel.setItemModel(&model);
    hmodel.add();
    model.setHeaderData(0, Qt::Horizontal, QVariant("Label"), Qt::DisplayRole);
    model.setHeaderData(1, Qt::Horizontal, QVariant("Wert"), Qt::DisplayRole);
    model.setHeaderData(2, Qt::Horizontal, QVariant("Farbe"), Qt::DisplayRole);
    view.setModel(&hmodel);

    view.connect(&model,
                 SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
                 SLOT(repaint()));

    QTableView tableview;
    tableview.setModel(&model);

    QPushButton button("Hinzufuegen");
    hmodel.connect(&button, SIGNAL(clicked()), SLOT(add()));

    layout.addWidget(&view, 50);
    layout.addWidget(&tableview, 50);
    layout.addWidget(&button, 50);
    wmain.setLayout(&layout);
    wmain.show();
    return app.exec();
}
