/* JHistMainWindow.cpp - Implementation of the main window.
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
// Include the xpm, and make the strings const to avoid warnings from C++
#define static static const
#include "QHisto.xpm"
#undef static

#include "JHistMainWindow.hpp"
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QApplication>
#include <QHeaderView>
#include <QPrinter>
#include <QSplitter>
#include <QCloseEvent>

JHistMainWindow::JHistMainWindow()
    : button_add("&Add"), button_remove("&Remove")
{
    setWindowIcon(QIcon(QPixmap(QHisto_xpm)));
    histview.setModel(&model);
    tableview.horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableview.setModel(&model);
    // Add a new row if the 'Add' button is clicked.
    model.connect(&button_add, SIGNAL(clicked()), SLOT(add()));
    connect(&button_remove, SIGNAL(clicked()), SLOT(remove()));

    QVBoxLayout *layout = new QVBoxLayout;
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(&histview);
    splitter->addWidget(&tableview);
    layout->addWidget(splitter);

    QHBoxLayout *buttons = new QHBoxLayout;
    layout->addLayout(buttons);
    buttons->addWidget(&button_add, 50);
    buttons->addWidget(&button_remove, 50);
    /* FIXME: Must be deleted */
    QMenuBar *menuBar = new QMenuBar;
    QMenu *file = menuBar->addMenu(tr("&File"));
    file->addAction(QIcon::fromTheme("document-new"), tr("&New"), this,
                    SLOT(clear()), QKeySequence::New);
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-open"), tr("&Open"), this,
                    SLOT(open()), QKeySequence::Open);
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-save"), tr("&Save"), this,
                    SLOT(save()), QKeySequence::Save);
    file->addAction(QIcon::fromTheme("document-save-as"), tr("Save &As"),
                    this, SLOT(saveAs()), QKeySequence::SaveAs);
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-print"), tr("&Print"), this,
                    SLOT(print()), QKeySequence::Print);
    file->addSeparator();
    file->addAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this,
                    SLOT(close()), QKeySequence::Quit);
    QMenu *help = menuBar->addMenu(tr("&Help"));
    help->addAction(QIcon::fromTheme("help-about-qt"), tr("About &Qt"), qApp,
                    SLOT(aboutQt()));
    help->addAction(QIcon::fromTheme("help-about"), tr("&About"), this,
                    SLOT(about()));
    layout->setMenuBar(menuBar);
    setLayout(layout);
    tableview.setItemDelegateForColumn (2, &color_delegate);

    setWindowTitle("QHisto");
    connect(&model, SIGNAL(changed()), this, SLOT(setChanged()));
    isChanged = false;
}

void JHistMainWindow::setChanged() {
    isChanged = true;

    if (windowTitle().at(0) != '*')
        setWindowTitle(QString("*%1").arg(windowTitle()));
}

void JHistMainWindow::closeEvent(QCloseEvent *event)
{
    if (isChanged && maybeSave())
        save();

    event->accept();
}

bool JHistMainWindow::maybeSave() {
    QMessageBox::StandardButton button;
    button = QMessageBox::question (this, "Do you want to save?",
                                    tr("Your data has been modified. Save?"),
                                    QMessageBox::Yes | QMessageBox::No);
    return (button == QMessageBox::Yes);
}

void JHistMainWindow::about()
{
    QMessageBox::about(this, tr("QHisto"),
                       tr("<p><b>QHisto</b> draws histograms. It sports "
                          "a simple flat file storage format, printing, "
                          "and PDF/PS-export.</p>"
                          "<p><b>QHisto</b> is licensed under the terms of "
                          "the MIT/X11 license, see the code for details.</p>"
                          "<p>Copyright &copy; 2010 Julian Andres Klode</p>"));
}

void JHistMainWindow::remove()
{
    QItemSelectionModel *selection = tableview.selectionModel();
    QModelIndexList index = selection->selectedRows();
    for (int i=index.size() - 1 ; i >= 0 ; i--)
        model.removeRow(index[i].row());
}

void JHistMainWindow::open(QString defName)
{
    if (isChanged && maybeSave())
        save();
    if (defName.isEmpty())
        fileName = QFileDialog::getOpenFileName(this, tr("Save File"), "",
                                                tr("QHisto files (*.qhisto)"));
    else
        fileName = defName;

    if (fileName.isEmpty())
        return;
    try {
        model.readFromFile(fileName);
    } catch (QString eString) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("Error while opening"), eString,
                                      QMessageBox::Retry | QMessageBox::Ignore);

        if (reply == QMessageBox::Retry)
            return open(fileName);
    }
    setWindowTitle(QString("%1 - QHisto").arg(fileName));
    isChanged = false;
}

void JHistMainWindow::save()
{
    if (fileName.isEmpty())
        return saveAs();

    try {
        model.writeToFile(fileName);
    } catch (QString eString) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("Error while saving"), eString,
                                      QMessageBox::Retry | QMessageBox::Ignore);
        if (reply == QMessageBox::Retry)
            return save();
    }
    isChanged = false;
    setWindowTitle(QString("%1 - QHisto").arg(fileName));
}

void JHistMainWindow::clear()
{
    if (isChanged && maybeSave())
        save();
    model.clear();
    fileName = "";
    isChanged = false;
    setWindowTitle(QString("QHisto"));
}

void JHistMainWindow::saveAs()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                            tr("QHisto files (*.qhisto)"));
    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".qhisto"))
        fileName += ".qhisto";

    try {
        model.writeToFile(fileName);
    } catch (QString eString) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::critical(this, tr("Error while saving"), eString,
                                      QMessageBox::Retry | QMessageBox::Ignore);

        if (reply == QMessageBox::Retry)
            return saveAs();
    }
    isChanged = false;
    setWindowTitle(QString("%1 - QHisto").arg(fileName));
}

void JHistMainWindow::print()
{
    QPrintPreviewDialog dialog;
    QPrinter *printer = dialog.printer();
    printer->setCreator("QHisto 1 (C) 2010 Julian Andres Klode");
    if (!fileName.isEmpty())
        printer->setDocName(QFileInfo(fileName).baseName());
    else
        printer->setDocName("QHisto");

    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), &histview,
            SLOT(paintOnTo(QPrinter*)));
    dialog.exec();
}
