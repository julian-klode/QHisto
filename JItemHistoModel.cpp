#include <cstddef>
#include <QString>
#include <QColor>
#include <QStandardItem>
#include "JItemHistoModel.hpp"

void JItemHistoModel::setItemModel(QAbstractItemModel * items)
{
    this->items = items;
}

int JItemHistoModel::size()
{
    return this->items->rowCount();
}

void JItemHistoModel::add()
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

QString JItemHistoModel::getLabel(int index)
{
    return items->data(items->index(index, 0), Qt::DisplayRole).toString();
}

double JItemHistoModel::getValue(int index)
{
    return items->data(items->index(index, 1), Qt::DisplayRole).toDouble();
}

QColor JItemHistoModel::getColor(int index)
{
    return QColor(items->data(items->index(index, 2), Qt::DisplayRole).
                  toString());
}
