#include <cstddef>
#include <QString>
#include <QColor>
#include "JItemHistoModel.hpp"

void JItemHistoModel::setItemModel(QAbstractItemModel *items)
{
    this->items = items;
}

size_t JItemHistoModel::size()
{
    return items->rowCount();
}

QColor JItemHistoModel::getColor(size_t index)
{
    return QColor(items->data(items->index(index, 3), Qt::DisplayRole).toString());
}

QString JItemHistoModel::getLabel(size_t index)
{
    return items->data(items->index(index, 1), Qt::DisplayRole).toString();
}

double JItemHistoModel::getValue(size_t index)
{
    return items->data(items->index(index, 2), Qt::DisplayRole).toDouble();
}
