/* JItemHistModel.cpp - Implementation of the JItemHistModel class.
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

#include <QString>
#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>
#include "JItemHistModel.hpp"

void JItemHistModel::setItemModel(QAbstractItemModel * items)
{
    this->items = items;
}

int JItemHistModel::size()
{
    return this->items->rowCount();
}

void JItemHistModel::add()
{
    QStandardItemModel *imodel = qobject_cast < QStandardItemModel * >(items);
    if (imodel) {
        QList < QStandardItem * >list;
        list << new QStandardItem("");
        list << new QStandardItem("");
        list << new QStandardItem("green");
        imodel->appendRow(list);
    }
}

QString JItemHistModel::getLabel(int index)
{
    return items->data(items->index(index, 0), Qt::DisplayRole).toString();
}

double JItemHistModel::getValue(int index)
{
    return items->data(items->index(index, 1), Qt::DisplayRole).toDouble();
}

QColor JItemHistModel::getColor(int index)
{
    return QColor(items->data(items->index(index, 2), Qt::DisplayRole).
                  toString());
}
