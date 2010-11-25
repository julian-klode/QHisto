// Include the xpm, and make the strings const to avoid warnings from C++
#define static static const
#include "QHisto.xpm"
#undef static

#include "JHistMainWindow.hpp"
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>

JHistMainWindow::JHistMainWindow()
    : button_add("Add")
{
    setWindowIcon(QIcon(QPixmap(QHisto_xpm)));
    histview.setModel(&model);
    tableview.setModel(&static_cast<QAbstractItemModel&>(model));
    // Repaint the histogram if the data changes.
    histview.connect(&static_cast<QAbstractItemModel&>(model),
                      SIGNAL(dataChanged(const QModelIndex&,
                                         const QModelIndex&)),
                      SLOT(repaint()));
    // Add a new row if the 'Add' button is clicked.
    model.connect(&button_add, SIGNAL(clicked()), SLOT(add()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(&histview, 50);
    layout->addWidget(&tableview, 50);
    layout->addWidget(&button_add, 50);
    /* FIXME: Must be deleted */
    QMenuBar *menuBar = new QMenuBar;
    QMenu *help = menuBar->addMenu(tr("&Help"));
    help->addAction(QIcon::fromTheme("help-about"), "&About", this, SLOT(about()));
    layout->setMenuBar(menuBar);
    setLayout(layout);
    tableview.setItemDelegateForColumn (2, &color_delegate);
}

void JHistMainWindow::about()
{
    QMessageBox::about(this, tr("QHisto"),
                       tr("<b>QHisto</b> draws histograms."));
}