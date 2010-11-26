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
#include <QStandardItemModel>

class QAbstractItemModel;

/**
 * \brief A JHistModel representing a 3-column QAbstractItemModel.
 *
 * This class represents a 3-column QAbstractItemModel as a JHistModel,
 * allowing it to be used by JHistView.
 */
class JItemHistModel : public JHistModel {
    Q_OBJECT

public:
    /** \brief Construct a new JItemHistModel. */
    JItemHistModel();
    /** \brief Return the label stored at row 'index', first column. */
    QString getLabel(int index);
    /** \brief Return the value stored at row 'index', second column. */
    double getValue(int index);
    /** \brief Return the color stored at row 'index', third column. */
    QColor getColor(int index);
    /** \brief Returns the number of rows in the model */
    int size();

    /**
     * \brief Operator to let the object be a QAbstractItemModel as well.
     *
     * When used in constructs where a reference to a QAbstractItemModel
     * is required, objects of this class automatically become one.
     */
    operator QAbstractItemModel& ();

public Q_SLOTS:
    /**
     * \brief Slot that inserts an empty row.
     *
     * Add a new empty row. This function only works if the model is
     * used to expose the data of a QStandardItemModel.
     */
    void add();

    /**
     * \brief Remove all rows.
     *
     * This basically calls clear() on the storage.
     */
    void clear();

protected:
    /**
     * \brief Create a new row.
     *
     * Add a new pre-populated row. This is helpful in
     * case data from somewhere else shall be read.
     */
    void add(QString label, double value, QColor color);

private:
    /**
     * \brief The model this model wraps.
     *
     * This is a 3-column model containing the data. The first
     * column represents the label, the second one represents
     * the value, and the third one represents the color.
     */
    QStandardItemModel items;
};

#endif /* J_ITEM_HISTO_MODEL_H */
