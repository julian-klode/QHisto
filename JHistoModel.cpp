#include <cstddef>
#include <map>
#include <QString>
#include "JHistoModel.hpp"


size_t JHistoModel::size()
{
    return 2;
}

QColor JHistoModel::getColor(size_t index)
{
    switch(index) {
    case 0:
        return Qt::red;
    case 1:
        return Qt::green;
    default:
        return Qt::red;
    }
}

QString JHistoModel::getLabel(size_t index)
{
    switch(index) {
    case 0:
        return "a";
    case 1:
        return "b";
    default:
        return "c";
    }
}

double JHistoModel::getValue(size_t index)
{
    switch(index) {
    case 0:
        return 1.742;
    case 1:
        return 3.14;
    default:
        return 100;
    }
}
