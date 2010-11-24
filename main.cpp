#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "JHistoModel.hpp"
#include "JHistoView.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget main;
    QVBoxLayout layout;
    
    JHistoView view(&main);
    JHistoModel model;
    view.setModel(&model);

    //layout.addWidget(&view);
    main.setLayout(&layout);
    main.show();
    return app.exec();
}
