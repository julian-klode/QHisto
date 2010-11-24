/* main.cpp - Implementation of the main function.
 *
 * Copyright (C) 2010 Julian Andres Klode <jak@jak-linux.org>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "JItemHistModel.hpp"
#include "JHistView.hpp"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>

// Include the xpm, and make the strings const to avoid warnings from C++
#define static static const
#include "QHisto.xpm"
#undef static

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    JItemHistModel model;
    QVBoxLayout layout;
    QWidget wmain;
    JHistView histoview;
    QTableView tableview;
    QPushButton button("Hinzufuegen");

    app.setWindowIcon(QIcon(QPixmap(QHisto_xpm)));

    histoview.setModel(&model);
    tableview.setModel(&static_cast<QAbstractItemModel&>(model));

    // Repaint the histogram if the data changes.
    histoview.connect(&static_cast<QAbstractItemModel&>(model),
                      SIGNAL(dataChanged(const QModelIndex&,
                                         const QModelIndex&)),
                      SLOT(repaint()));

    // Add a new row if the 'Add' button is clicked.
    model.connect(&button, SIGNAL(clicked()), SLOT(add()));

    // Fill our layout and set it as the layout of the window (TODO: why 50?)
    layout.addWidget(&histoview, 50);
    layout.addWidget(&tableview, 50);
    layout.addWidget(&button, 50);
    wmain.setLayout(&layout);

    wmain.show();
    return app.exec();
}
