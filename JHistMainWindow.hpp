/* JHistMainWindow.hpp - Interface of the main window.
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
#include "JHistModel.hpp"
#include "JHistView.hpp"
#include "JColorItemDelegate.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QPrintPreviewDialog>

/**
 * \brief The main window.
 */
class JHistMainWindow : public QWidget {
    Q_OBJECT

public:
    JHistMainWindow();

private Q_SLOTS:
    /** \brief Called when the 'Help|About' menu item is activated. */
    void about();
    /** \brief Called when the 'File|New' menu item is activated. */
    void clear();
    /** \brief Called when the 'Remove' button is activated. */
    void remove();
    /** \brief Called when the 'File|Save' menu item is activated. */
    void save();
    /** \brief Called when the 'File|Save As' menu item is activated. */
    void saveAs();
    /** \brief Called when the 'File|Print' menu item is activated. */
    void print();
    void setChanged();
public Q_SLOTS:
    /**
     * \brief Open a file for reading.
     *
     * If the argument defName is an empty string, open a file chooser
     * and open the file chosen by the user. If defName is not empty,
     * open the file with that name. If the file cannot be opened, ask
     * the user what to do.
     *
     * \param defName The name of the file to open, empty string causes
     *                a file chooser to ask the user for a file.
     */
    void open(QString defName = "");
private:
    JHistModel model;
    JHistView histview;
    QTableView tableview;
    QPushButton button_add;
    QPushButton button_remove;
    JColorItemDelegate color_delegate;
    QString fileName;
    /** \brief true if the model has changed since last save. */
    bool isChanged;
    /** \brief ask the user whether they want to save. */
    bool maybeSave();
    /** \brief called when window should be closed. */
    void closeEvent(QCloseEvent *event);
};
