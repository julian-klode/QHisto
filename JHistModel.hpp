/* JHistModel.hpp - Implementation of JHistModel.
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
#include <QAbstractTableModel>
#include <QList>
#include <QColor>

/**
 * \brief Opaque type representing an item in the model.
 *
 * The layout of the type is internal to the model.
 */
struct JHistItem;

/**
 * \brief Model representing data in a histogram.
 *
 * This model represents a 3 column table. The first column is a string
 * containing a label, the second column a double representing the value
 * of the bar, and the third column contains a QColor that determines the
 * color the bar will receive.
 *
 * Methods of which the documentation starts with "Override:" implement
 * pure virtual methods of the base class.
 */
class JHistModel : public QAbstractTableModel {
    Q_OBJECT
public:
    ~JHistModel();

    /** \brief Override: The number of columns (== 3 if parent is invalid). */
    int columnCount(const QModelIndex &parent) const;
    /** \brief Override: The number of rows. */
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    /**
     * \brief Override: The data at the model index.
     *
     * If index.column() == 1, return a string; If index.column() == 2,
     * return a string representing a double; if index.column() == 3,
     * return a QColor. Do this only if role is Qt::DisplayRole or
     * Qt::EditRole. Otherwise, return an invalid variant.
     */
    QVariant data(const QModelIndex &index, int role) const;

    /**
     * \brief Override: Set the data at the index.
     *
     * The QVariant should be representable as string for column 1, as
     * double for column 2, and as QColor for column 3. Role shall be
     * Qt::DisplayRole or Qt::EditRole, otherwise nothing is done.
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    /** \brief Override: The options supported by the items. */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /** \brief Override: Insert 'count' new rows at 'row'. */
    bool insertRows(int row, int count, const QModelIndex &parent=QModelIndex());
    /** \brief Override: Remove 'count' rows at 'row'. */
    bool removeRows(int row, int count, const QModelIndex &parent=QModelIndex());
    /** \brief Override: Return the headers. */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * \brief Read the data from the given file.
     *
     * Read a file according to the format specified in QHisto(5).
     *
     * \throw QString describing the error if something failed.
     */
    void readFromFile(const QString &filename) throw(QString);

    /**
     * \brief Write the data to the given file.
     *
     * Write a file according to the format specified in QHisto(5).
     *
     * \throw QString describing the error if something failed.
     */
    void writeToFile(const QString &filename) const throw(QString);

public slots:
    /** \brief Append an empty new row */
    void add() { insertRow(rowCount()); }
    /** \brief Remove all rows in the model. */
    void clear() { removeRows(0, rowCount()); }

signals:
    /** \brief Emitted whenever something changes. */
    void changed();

private:
    /** \brief The items. */
    QList<JHistItem*> items;
};
