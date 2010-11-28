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

JItemHistModel::JItemHistModel()
    : items(0, 3)
{
    items.setHeaderData(0, Qt::Horizontal, QVariant("Label"), Qt::DisplayRole);
    items.setHeaderData(1, Qt::Horizontal, QVariant("Wert"), Qt::DisplayRole);
    items.setHeaderData(2, Qt::Horizontal, QVariant("Farbe"), Qt::DisplayRole);

    connect(&items, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            SLOT(emitChanged()));
    connect(&items, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            SLOT(emitChanged()));
};

int JItemHistModel::size() const
{
    return this->items.rowCount();
}

void JItemHistModel::clear()
{
    items.removeRows(0, size());
}

void JItemHistModel::add(QString label, double value, QColor color)
{
    QList <QStandardItem *> list;
    list << new QStandardItem(label);
    list << new QStandardItem(QString::number(value));
    list << new QStandardItem(color.name());
    items.appendRow(list);
}

void JItemHistModel::add()
{
    QList <QStandardItem *> list;
    list << new QStandardItem("");
    list << new QStandardItem("");
    list << new QStandardItem("green");
    items.appendRow(list);
}

QString JItemHistModel::getLabel(int index) const
{
    return items.data(items.index(index, 0), Qt::DisplayRole).toString();
}

double JItemHistModel::getValue(int index) const
{
    return items.data(items.index(index, 1), Qt::DisplayRole).toDouble();
}

QColor JItemHistModel::getColor(int index) const
{
    return items.data(items.index(index, 2),Qt::DisplayRole).value<QColor>();
}

QAbstractItemModel *JItemHistModel::itemModel()
{
    return &items;
}

void JItemHistModel::emitChanged() {
    emit changed();
}
