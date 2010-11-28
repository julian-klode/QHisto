/* JHistModel.cpp - Implementation of JHistModel.
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
#include <QModelIndex>
#include <QVariant>
#include <QColor>
#include <QFile>

struct JHistItem {
    QString label;
    double  value;
    QColor  color;
    JHistItem(const QString& label, double value, const QColor& color)
        : label(label), value(value), color(color)
    {
    }
};

int JHistModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

int JHistModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : items.size();
}

QVariant JHistModel::data (const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return items[index.row()]->label;
        case 1: return items[index.row()]->value;
        case 2: return items[index.row()]->color;
        }
    }
    return QVariant();
}

bool JHistModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole && role != Qt::DisplayRole)
        return false;

    switch (index.column()) {
    case 0: items[index.row()]->label = value.toString(); break;
    case 1: items[index.row()]->value = value.toDouble(); break;
    case 2: items[index.row()]->color = value.value<QColor>(); break;
    default:
        return false;
    }

    emit dataChanged(index, index);
    emit changed();
}

Qt::ItemFlags JHistModel::flags(const QModelIndex & /* index */) const
{
    return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool JHistModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    beginInsertRows(parent, row, count);
    while (count-- > 0)
        items.insert(row, new JHistItem("", 0, "green"));
    endInsertRows();
    emit changed();
    return true;
}

bool JHistModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    beginRemoveRows(parent, row, count);
    while (count--) {
        delete items[row];
        items.removeAt(row);
    }
    endRemoveRows();
    emit changed();
    return true;
}

QVariant JHistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Label";
        case 1: return "Value";
        case 2: return "Color";
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

double JHistModel::maximumValue() const
{
    double max = 0;
    for (int i = 0; i < items.size(); i++)
        max = qMax(max, items[i]->value);
    return max;
}

double JHistModel::minimumValue() const
{
    double min = 0;
    for (int i = 0; i < items.size(); i++)
        min = qMin(min, items[i]->value);
    return min;
}


/* Now comes the simple read-write support */

/**
 * \brief Escape or unescape the given string.
 *
 * Escape or unescape a string according to the specification
 * in QHisto(5).
 *
 * \param reading If true, escape sequences will be replaced by
 *                characters. Otherwise, characters will be escaped.
 */
static QString escape(QString string, bool reading) {
    static const struct {
        char character[2];
        char escaped[3];
    } sequences[] = { { "\\", "\\\\" }, { "\n", "\\n"}, {"\r", "\\r" },
                      { "\t", "\\t" }, { "#", "\\#"} };
    for (unsigned i = 0; i < sizeof(sequences) / sizeof(sequences[0]); i++) {
        if (reading)
            string.replace(sequences[i].escaped, sequences[i].character);
        else
            string.replace(sequences[i].character, sequences[i].escaped);
    }
    return string;
}

void JHistModel::readFromFile(const QString &filename) throw (QString)
{
    QFile device(filename);
    QByteArray line;
    if (!device.open(QIODevice::ReadOnly | QIODevice::Text))
        throw device.errorString();

    clear();

    QList<JHistItem*> newItems = items;
    while ((line = device.readLine()).size()) {
        if (line.at(0) == '#')
            continue;
        if (line.at(line.size() -1) == '\n')
            line.truncate(line.size() - 1);
        QList<QByteArray> list = line.split('\t');
        JHistItem *item;
        item = new JHistItem(escape(QString::fromUtf8(list[0]), true),
                             list[1].toDouble(), QString(list[2]));
        newItems.append(item);
    }
    beginInsertRows(QModelIndex(), 0, newItems.size());
    items = newItems;
    endInsertRows();
    emit changed();
}

void JHistModel::writeToFile(const QString &filename) const throw (QString)
{
    QFile device(filename);
    if (!device.open(QIODevice::WriteOnly))
        throw device.errorString();
    for (int i=0; i < items.size(); i++) {
        device.write(escape(items[i]->label, false).toUtf8());
        device.write("\t");
        device.write(QString::number(items[i]->value).toUtf8());
        device.write("\t");
        device.write(items[i]->color.name().toUtf8());
        device.write("\n");
    }
}
