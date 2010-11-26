// Include the xpm, and make the strings const to avoid warnings from C++
#define static static const
#include "QHisto.xpm"
#undef static

#include "JHistMainWindow.hpp"
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QItemSelectionModel>

JHistMainWindow::JHistMainWindow()
    : button_add("Add"), button_remove("Remove")
{
    setWindowIcon(QIcon(QPixmap(QHisto_xpm)));
    histview.setModel(&model);
    tableview.setModel(&static_cast<QAbstractItemModel&>(model));
    // Repaint the histogram if the data changes.
    histview.connect(&static_cast<QAbstractItemModel&>(model),
                      SIGNAL(dataChanged(const QModelIndex&,
                                         const QModelIndex&)),
                      SLOT(repaint()));
    histview.connect(&static_cast<QAbstractItemModel&>(model),
                      SIGNAL(rowsRemoved (const QModelIndex&, int, int)),
                      SLOT(repaint()));
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

void JHistMainWindow::remove()
{
    QItemSelectionModel *selection = tableview.selectionModel();
    QModelIndexList index = selection->selectedRows();
    for (int i=0; i < index.size(); i++)
        static_cast<QAbstractItemModel&>(model).removeRow(index[i].row());
}
