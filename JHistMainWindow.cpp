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

JHistMainWindow::JHistMainWindow()
    : button_add("&Add"), button_remove("&Remove")
{
    setWindowIcon(QIcon(QPixmap(QHisto_xpm)));
    histview.setModel(&model);
    tableview.horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableview.setModel(&static_cast<QAbstractItemModel&>(model));
    // Repaint the histogram if the data changes.
    histview.connect(&model, SIGNAL(changed()), SLOT(repaint()));
    // Add a new row if the 'Add' button is clicked.
    model.connect(&button_add, SIGNAL(clicked()), SLOT(add()));
    connect(&button_remove, SIGNAL(clicked()), SLOT(remove()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(&histview, 50);
    layout->addWidget(&tableview, 50);

    QHBoxLayout *buttons = new QHBoxLayout;
    layout->addLayout(buttons);
    buttons->addWidget(&button_add, 50);
    buttons->addWidget(&button_remove, 50);
    /* FIXME: Must be deleted */
    QMenuBar *menuBar = new QMenuBar;
    QMenu *file = menuBar->addMenu(tr("&File"));
    file->addAction(QIcon::fromTheme("document-new"), "&New", this, SLOT(clear()),
                    QKeySequence(tr("Ctrl+N", "File|New")));
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-open"), "&Open", this, SLOT(open()),
                    QKeySequence(tr("Ctrl+O", "File|Open")));
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-save"), "&Save", this, SLOT(save()),
                    QKeySequence(tr("Ctrl+S", "File|Save")));
    file->addAction(QIcon::fromTheme("document-save-as"), "Save &As", this, SLOT(saveAs()),
                    QKeySequence(tr("Ctrl+Shift+S", "File|Save As")));
    file->addSeparator();
    file->addAction(QIcon::fromTheme("document-print"), "&Print", this, SLOT(print()),
                    QKeySequence(tr("Ctrl+P", "File|Print")));
    file->addSeparator();
    file->addAction(QIcon::fromTheme("application-exit"), "&Quit", this, SLOT(close()),
                    QKeySequence(tr("Ctrl+Q", "File|Quit")));
    QMenu *help = menuBar->addMenu(tr("&Help"));
    help->addAction(QIcon::fromTheme("help-about-qt"), "About &Qt", qApp,
                    SLOT(aboutQt()));
    help->addAction(QIcon::fromTheme("help-about"), "&About", this,
                    SLOT(about()));
    layout->setMenuBar(menuBar);
    setLayout(layout);
    tableview.setItemDelegateForColumn (2, &color_delegate);
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
    for (int i=0; i < index.size(); i++)
        static_cast<QAbstractItemModel&>(model).removeRow(index[i].row());
}

void JHistMainWindow::open(QString defName)
{
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
    histview.repaint();
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
}

void JHistMainWindow::clear()
{
    model.clear();
    fileName = "";
    histview.repaint();
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
