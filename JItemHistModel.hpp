/* JItemHistModel.hpp - Interface of the JItemHistModel class.
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

#ifndef J_ITEM_HISTO_MODEL_H
#define J_ITEM_HISTO_MODEL_H

#include "JHistModel.hpp"

#include <QString>
#include <QColor>

class QAbstractItemModel;

/**
 * \brief A JHistModel representing a 3-column QAbstractItemModel.
 *
 * This class represents a 3-column QAbstractItemModel as a JHistModel,
 * allowing it to be used by JHistView.
 */
class JItemHistModel : public JHistModel {
    Q_OBJECT

    /**
     * \brief The model this model wraps.
     *
     * This is a 3-column model containing the data. The first
     * column represents the label, the second one represents
     * the value, and the third one represents the color.
     */
    QAbstractItemModel *items;

public:
    /** \brief Set the model containing the items. */
    void setItemModel(QAbstractItemModel *items);
    /** \brief Return the label stored at row 'index', first column. */
    QString getLabel(int index);
    /** \brief Return the value stored at row 'index', second column. */
    double getValue(int index);
    /** \brief Return the color stored at row 'index', third column. */
    QColor getColor(int index);
    /** \brief Returns the number of rows in the model */
    int size();

public Q_SLOTS:
    /** \brief Slot that inserts an empty row.
     *
     * Add a new empty row. This function only works if the model is
     * used to expose the data of a QStandardItemModel.
     */
    void add();
};

#endif /* J_ITEM_HISTO_MODEL_H */
